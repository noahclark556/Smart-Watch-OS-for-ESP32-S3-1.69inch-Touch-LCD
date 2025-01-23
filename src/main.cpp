#include "lvgl.h"
#include "Arduino_GFX_Library.h"
#include "Arduino_DriveBus_Library.h"
#include "pin_config.h"
#include "layout/layout.h"
#include "inits/wifi_init.h"
#include "util/util.h"

// Modify lv_conf.h to adjust lower level stuff like the perf monitor, etc.
// its in .pio/libdeps

// Modify partition table to free up space
// USBSerial is defined in util.cpp


Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCK, LCD_MOSI);

Arduino_GFX *gfx = new Arduino_ST7789(bus, LCD_RST /* RST */,
                                      0 /* rotation */, true /* IPS */, LCD_WIDTH, LCD_HEIGHT, 0, 20, 0, 0);

std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus =
    std::make_shared<Arduino_HWIIC>(IIC_SDA, IIC_SCL, &Wire);

void Arduino_IIC_Touch_Interrupt(void);

std::unique_ptr<Arduino_IIC> CST816T(new Arduino_CST816x(IIC_Bus, CST816T_DEVICE_ADDRESS,
                                                         TP_RST, TP_INT, Arduino_IIC_Touch_Interrupt));

void Arduino_IIC_Touch_Interrupt(void)
{
  CST816T->IIC_Interrupt_Flag = true;
}

#define EXAMPLE_LVGL_TICK_PERIOD_MS 2

uint32_t screenWidth;
uint32_t screenHeight;

static lv_disp_draw_buf_t draw_buf;

#if LV_USE_LOG != 0
// for serial debugging, need to figure this out better
void log_print(const char *buf)
{
  Serial.printf(buf);
  Serial.flush();
}
#endif

// flush display (makes things appear on screen)
// driver, area, pixel buffer
void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
  gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

  lv_disp_flush_ready(disp);
}

void increase_lvgl_tick(void *arg)
{
  // tick lvgl
  lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

static uint8_t count = 0;
void increase_reboot(void *arg)
{
  count++;
  if (count == 30)
  {
    esp_restart();
  }
}

// touchpad reader
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  int32_t touchX = CST816T->IIC_Read_Device_Value(CST816T->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);
  int32_t touchY = CST816T->IIC_Read_Device_Value(CST816T->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y);

  if (CST816T->IIC_Interrupt_Flag == true)
  {
    CST816T->IIC_Interrupt_Flag = false;
    data->state = LV_INDEV_STATE_PR;

    // set initial coords
    if (touchX >= 0 && touchY >= 0)
    {
      data->point.x = touchX;
      data->point.y = touchY;

      USBSerial.printf("Data x: %d, Data y: %d\n", touchX, touchY);
    }
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
}

void setup()
{
  USBSerial.begin(115200); // serial debug prep
  // Wire.begin();
  //  Serial.begin(115200);
  //   Wire.begin();
  //   Serial.println("Scanning I2C bus...");
  //   for (uint8_t address = 1; address < 127; address++)
  //   {
  //       Wire.beginTransmission(address);
  //       if (Wire.endTransmission() == 0)
  //       {
  //           Serial.printf("Found device at 0x%02X\n", address);
  //       }
  //   }
  //   Serial.println("Scan complete.");

  while (CST816T->begin() == false)
  {
    USBSerial.println("CST816T initialization fail");
    delay(2000);
  }
  USBSerial.println("CST816T initialization successfully");


  CST816T->IIC_Write_Device_State(CST816T->Arduino_IIC_Touch::Device::TOUCH_DEVICE_INTERRUPT_MODE,
                                  CST816T->Arduino_IIC_Touch::Device_Mode::TOUCH_DEVICE_INTERRUPT_PERIODIC);

  gfx->begin();
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);

  screenWidth = gfx->width();
  screenHeight = gfx->height();

  lv_init();

  lv_color_t *buf1 = (lv_color_t *)heap_caps_malloc(screenWidth * screenHeight / 4 * sizeof(lv_color_t), MALLOC_CAP_DMA);

  lv_color_t *buf2 = (lv_color_t *)heap_caps_malloc(screenWidth * screenHeight / 4 * sizeof(lv_color_t), MALLOC_CAP_DMA);

#if LV_USE_LOG != 0
  lv_log_register_print_cb(log_print); // register print function for debugging
#endif

  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, screenWidth * screenHeight / 4);

  // init display
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  
  // display res
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // init input drivers
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touchpad_read;
  lv_indev_drv_register(&indev_drv);

  // tick lvgl
  const esp_timer_create_args_t lvgl_tick_timer_args = {
      .callback = &increase_lvgl_tick,
      .name = "lvgl_tick"};

  const esp_timer_create_args_t reboot_timer_args = {
      .callback = &increase_reboot,
      .name = "reboot"};

  esp_timer_handle_t lvgl_tick_timer = NULL;
  esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer);
  esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000);

  initialize_wifi();
  initialize_layout();
}

void loop()
{
  lv_timer_handler();
  delay(5);
}