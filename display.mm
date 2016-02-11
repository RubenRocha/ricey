#ifdef __OBJC__
#import <CoreGraphics/CoreGraphics.h>
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#endif

#import "display.h"

float get_resolution_scale() {
    float H = [[NSScreen mainScreen] backingScaleFactor];
    return H;
};

