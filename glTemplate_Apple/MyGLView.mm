//
//  MyGLView.m
//  TestGL
//
//  Created by jonathan on 02/02/2014.
//  Copyright (c) 2014 jonathan. All rights reserved.
//
#import "MyGLView.h"
#import "Game.h"
#import "Common.h"
#import <GLUT/GLUT.h>

#define LOG_ME 0

@interface MyGLView()
@property (nonatomic, strong) NSTimer* renderTimer;
@property (nonatomic, assign) NSTimeInterval elapsedTime;
@property (nonatomic, strong) NSDate* startTime;
@end

@implementation MyGLView

- (id)initWithFrame:(NSRect)frame
{
    printf("%s\n",__func__);
    self = [super initWithFrame:frame];

    //below code helps optimize Open GL context
    // initialization for the best available resolution
    // important for Retina screens for example

    if (self) {
        [self wantsBestResolutionOpenGLSurface];
    }
    return self;
}

- (void)awakeFromNib {
    {
        if (LOG & LOG_ME) printf("%s\n",__func__);
        NSOpenGLPixelFormatAttribute attr[] = {
            NSOpenGLPFAOpenGLProfile,
            NSOpenGLProfileVersion3_2Core, // Needed if using opengl 3.2 you can comment this line out to use the old version.
            NSOpenGLPFAColorSize,     24,
            NSOpenGLPFAAlphaSize,     8,
            NSOpenGLPFAAccelerated,
            NSOpenGLPFADoubleBuffer,
            0
        };
        NSOpenGLPixelFormat *pix = [[NSOpenGLPixelFormat alloc] initWithAttributes:attr];
        self.pixelFormat = pix;
        self.renderTimer = [NSTimer timerWithTimeInterval:0.001
                                               target:self
                                             selector:@selector(timerFired:)
                                            userInfo:nil
                                             repeats:YES];

        [[NSRunLoop currentRunLoop] addTimer:self.renderTimer
                                     forMode:NSDefaultRunLoopMode];
        
        //Ensure timer fires during resize
        
        [[NSRunLoop currentRunLoop]
         addTimer:self.renderTimer
         forMode:NSEventTrackingRunLoopMode];
        self.startTime = [NSDate date];
        
    }
}

- (void)timerFired:(id)sender
{
    if (LOG & LOG_ME) printf("%s\n",__func__);
    float time = (float)[[NSDate date] timeIntervalSinceDate:self.startTime];
    Game::GetInstance().UpdateWithTime(time);
    [self setNeedsDisplay:YES];
}

- (void)prepareOpenGL
{
    if (LOG & LOG_ME) printf("%s\n",__func__);

    [super prepareOpenGL];
    // Synchronize buffer swaps with vertical refresh rate
    GLint swapInt = 1;
    [self.openGLContext setValues:&swapInt
                     forParameter:NSOpenGLCPSwapInterval];
    [self.openGLContext makeCurrentContext];

    Game::GetInstance().InitialiseWithSize((int)[self bounds].size.width, (int)[self bounds].size.height);


}


- (void) drawRect:(NSRect)dirtyRect {

    if (LOG & LOG_ME) printf("%s\n",__func__);
    //below code sets the viewport of Open GL context into
    //correct size (assuming resize, fullscreen operations may trigger change)

    NSRect backingBounds = [self convertRectToBacking:[self bounds]];
    glViewport(0,0, backingBounds.size.width, backingBounds.size.height);

    //our renderer's drawing routine
    [self.openGLContext makeCurrentContext];
    Game::GetInstance().SetSize((int)[self bounds].size.width, (int)[self bounds].size.height);
    Game::GetInstance().Render();
}

#pragma mark - responder

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (BOOL)resignFirstResponder {
    return YES;
}

- (BOOL)becomeFirstResponder {
    [self setNeedsDisplay:YES];
    return YES;
}


- (void)keyDown:(NSEvent *)theEvent {
    [self interpretKeyEvents:[NSArray arrayWithObjects:theEvent, nil]];
    if (theEvent.keyCode == 123) {
        Game::GetInstance().ProcessKeyDownEvent(123);
    }
    else if (theEvent.keyCode == 124) {
        //left arrow
        Game::GetInstance().ProcessKeyDownEvent(124);

    }
    else if (theEvent.keyCode == 126) {
        //up arrow
        Game::GetInstance().ProcessKeyDownEvent(126);

    }
    else if (theEvent.keyCode == 125) {
        //down arrow
        Game::GetInstance().ProcessKeyDownEvent(125);

    }

}
@end



