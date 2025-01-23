#ifndef IMG_NOAH_H
#define IMG_NOAH_H

#include <lvgl.h>

// IMPORTANT:
// Generate this shiz with https://lvgl.io/tools/imageconverter
	// Use V8 With LV_IMG_CF_RGB565A8
	// LV_Conf.h needs to be set to 16bit (should be already)
	// In the generated c file, change the const at the bottom to this format:
	// 	const lv_img_dsc_t IMG_NOAH = {
    // 		{ LV_IMG_CF_RGB565A8, 0, 0, 200, 279 },  // header
    // 			167400,                                // data_size
  	// 		IMG_NOAH_map                           // data
	// 	};
    // Also shrink the image down before using this converter. Shrink it to 10kb or less or shitty shit happens.
    // Use this for shrink: https://imageresizer.com/resize/download/6754e3dccfd1dec19a000984


extern const uint8_t IMG_NOAH_map[]; // Declare the image data
extern const lv_img_dsc_t IMG_NOAH;  // Declare the image descriptor

#endif // IMG_NOAH_H
