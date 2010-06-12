//
//  Measure.h
//  SpinWithController
//
//  Created by Loïc Séguin-Charbonneau on 10-06-10.
//  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface Measure : NSObject {
    float temperature;
    float data;
}

@property (assign) float temperature;
@property (assign) float data;
- (id)initWithTemperature:(float)temp data:(float)measure;

@end
