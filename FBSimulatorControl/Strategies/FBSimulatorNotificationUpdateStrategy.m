/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import "FBSimulatorNotificationUpdateStrategy.h"

#import <CoreSimulator/SimDevice.h>
#import <FBControlCore/FBControlCore.h>

#import "FBCoreSimulatorNotifier.h"
#import "FBSimulator.h"
#import "FBSimulatorEventSink.h"
#import "FBSimulatorSet.h"

@interface FBSimulatorNotificationUpdateStrategy ()

@property (nonatomic, weak, readonly) FBSimulatorSet *set;
@property (nonatomic, strong, readwrite) FBCoreSimulatorNotifier *notifier;

@end

@implementation FBSimulatorNotificationUpdateStrategy

#pragma mark Initializers

+ (instancetype)strategyWithSet:(FBSimulatorSet *)set
{
  FBSimulatorNotificationUpdateStrategy *strategy = [[self alloc] initWithSet:set];
  [strategy startNotifyingOfStateChanges];
  return strategy;
}

- (instancetype)initWithSet:(FBSimulatorSet *)set
{
  self = [super init];
  if (!self) {
    return nil;
  }

  _set = set;

  return self;
}

- (void)dealloc
{
  [self.notifier terminate];
  self.notifier = nil;
}

#pragma mark Private

- (void)startNotifyingOfStateChanges
{
  __weak typeof(self) weakSelf = self;
  self.notifier = [FBCoreSimulatorNotifier notifierForSet:self.set queue:dispatch_get_main_queue() block:^(NSDictionary *info) {
    SimDevice *device = info[@"device"];
    if (!device) {
      return;
    }
    NSNumber *newStateNumber = info[@"new_state"];
    if (!newStateNumber) {
      return;
    }
    [weakSelf device:device didChangeState:newStateNumber.unsignedIntegerValue];
  }];
}

- (void)device:(SimDevice *)device didChangeState:(FBSimulatorState)state
{
  FBiOSTargetQuery *query = [FBiOSTargetQuery udid:device.UDID.UUIDString];
  NSArray<FBSimulator *> *simulators = [self.set query:query];
  if (simulators.count != 1) {
    return;
  }
  [simulators.firstObject.eventSink didChangeState:state];
}

@end