//
//  Measure.m
//  SpinWithController
//
//  Created by Loïc Séguin-Charbonneau on 10-06-10.
//  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
//

#import "Measure.h"


@implementation Measure

@synthesize temperature;
@synthesize data;

- (id)initWithTemperature:(float)temp data:(float)measure {
    [super init];
    [self setTemperature:temp];
    [self setData:measure];
    return self;
}

@end
