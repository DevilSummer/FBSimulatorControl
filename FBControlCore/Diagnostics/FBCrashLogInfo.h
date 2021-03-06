/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import <Foundation/Foundation.h>

@class FBDiagnostic;
@class FBDiagnosticBuilder;

NS_ASSUME_NONNULL_BEGIN

/**
 The Process Type of the Crash Log
*/
typedef NS_OPTIONS(NSUInteger, FBCrashLogInfoProcessType) {
  FBCrashLogInfoProcessTypeSystem = 1 << 0, /** A Crash for a Simulator's System Application */
  FBCrashLogInfoProcessTypeApplication = 1 << 1, /** A Crash for an App in the Simulator **/
  FBCrashLogInfoProcessTypeCustomAgent = 1 << 2, /** A Crash for a Custom Launched Agent in the Simulator **/
};

/**
 Information about Crash Logs.
 */
@interface FBCrashLogInfo : NSObject <NSCopying>

#pragma mark Properties

/**
 The "Unique" name of the crash log.
 This is taken to be the the last path component of the crash log path.
 */
@property (nonatomic, copy, readonly) NSString *name;

/**
 The Path of the Crash Log.
 */
@property (nonatomic, copy, readonly) NSString *crashPath;

/**
 The identifier of the Crash Log.
 */
@property (nonatomic, copy, readonly) NSString *identifier;

/**
 The Path of the Executable Image.
 */
@property (nonatomic, copy, readonly) NSString *executablePath;

/**
 The Name of the Crashed Process.
 */
@property (nonatomic, copy, readonly) NSString *processName;

/**
 The Process Identifier of the Crashed Process/
 */
@property (nonatomic, assign, readonly) pid_t processIdentifier;

/**
 The Process Name of the Crashed Process's parent.
 */
@property (nonatomic, copy, readonly) NSString *parentProcessName;

/**
 The Process Identifier of the Crashed Process's parent.
 */
@property (nonatomic, assign, readonly) pid_t parentProcessIdentifier;

/**
 The date of the crash
 */
@property (nonatomic, copy, readonly) NSDate *date;

/**
 The Process Type of the Crash Log
 */
@property (nonatomic, assign, readonly) FBCrashLogInfoProcessType processType;

#pragma mark Helpers

/**
 The Diagnostics Report Path for the User.
 */
@property (nonatomic, class, copy, readonly) NSString *diagnosticReportsPath;

#pragma mark Initializers

/**
 Creates Crash Log Info from the specified crash log path.
 Returns nil on error.

 @param path the path to extract crash log info from.
 @return a Crash Log Info on success, nil otherwise.
 */
+ (nullable instancetype)fromCrashLogAtPath:(NSString *)path;

#pragma mark Public Methods

/**
 Constructs a FBDiagnostic instance from the Crash Log.

 @param builder the builder to populate the Crash Log into.
 */
- (FBDiagnostic *)toDiagnostic:(FBDiagnosticBuilder *)builder;

/**
 Determines whether the data represents a crash log.

 @param data the data to attempt to parse.
 @return YES if it is parsable, NO otherwise.
 */
+ (BOOL)isParsableCrashLog:(NSData *)data;

#pragma mark Bulk Collection

/**
 Collects all Crash Log Info from the Default Path.

 @param date the first date to search from.
 @return an Array of all found Crash Log info.
 */
+ (NSArray<FBCrashLogInfo *> *)crashInfoAfterDate:(NSDate *)date;

#pragma mark Predicates

/**
 A Predicate for FBCrashLogInfo that passes for all Crash Logs with certain process info.

 @param processID the Process ID of the Crash to Collect.
 @return a NSPredicate.
 */
+ (NSPredicate *)predicateForCrashLogsWithProcessID:(pid_t)processID;

/**
 A Predicate for FBCrashLogInfo that passes for all Crash Logs that are newer than the given date.

 @param date the start date.
 @return a NSPredicate.
 */
+ (NSPredicate *)predicateNewerThanDate:(NSDate *)date;

/**
 A Predicate for FBCrashLogInfo that matches a identifier.

 @param identifier the identifier to use.
 @return an NSPredicate
 */
+ (NSPredicate *)predicateForIdentifier:(NSString *)identifier;

/**
 A Predicate for FBCrashLogInfo that matches a name.

 @param name the names use.
 @return an NSPredicate
 */
+ (NSPredicate *)predicateForName:(NSString *)name;

@end

NS_ASSUME_NONNULL_END
