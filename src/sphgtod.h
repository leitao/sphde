/*
 * Copyright (c) 2012-2014 IBM Corporation.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *     IBM Corporation, Ryan S Arnold - New API.
 *     IBM Corporation, Steven J Munroe - Correct Intel implementation.
 */

#ifndef __SPH_GTOD_H_
#define __SPH_GTOD_H_

/*!
 * \file  sphgtod.h
 * \brief Functions to convert time stamp (sphtimer_t) values to
 * struct timeval value format with gettimeofday() epoch.
 *
 * This API is intended to support post processing time stamps
 * (including those generated by SPHLFLogger_t and SPHSinglePCQueue_t)
 * into gettimeofday() epoch and related formated time values.
 *
 * It is not intended as an exact replacement for gettimeofday()
 * because it may not include any NTP corrections for example,
 * but it will close and will be monotonic.
 */

#include <sys/time.h>
#include <sphtimer.h>

#ifdef __cplusplus
#define __C__ "C"
#else
#define __C__
#endif

/*!
 * \brief Return the timebase converted to gettimeofday struct timeval.
 *
 * Returns a fast emulation of the values returned by gettimeofday computed
 * from a queried machine timebase register value.
 *
 * The timezone is not currently honored in the conversion.
 *
 * @param tv address of the struct timeval buffer.
 * @param tz address of the stuct timezone or NULL.
 * @return The timebase converted to gettimeofday in struct timeval.
 */

extern __C__ int
sphgtod (struct timeval *tv, struct timezone *tz);

/*!
 * \brief Return the timebase-to-gettimeofday conversion factor.
 *
 * As the timebase (sphtimer_t) is a fast hardware timer or something
 * similar to "uptime" we need to allow for logs that persist across
 * reboot. By saving this conversion factor in the log we can use it
 * during post processing of the log to get the corrected gettimeofday
 * value for formated time values.
 *
 * This allow the logger to run as fast as possible by postponing the
 * timebase to gettimeofday conversion until post processing.
 *
 * @return The timebase to gettimeofday conversion factor.
 */

extern __C__ sphtimer_t
sphget_gtod_conv_factor (void);

/*!
 * \brief Return the timebase converted to gettimeofday struct timeval.
 *
 * As the timebase (sphtimer_t) is a fast hardware timer or something
 * similar to "uptime" we need to allow for logs that persist across
 * reboot. By saving the conversion factor (via sphget_gtod_conv_factor())
 * in the log we can use it (via this API) during post processing of
 * the log to get the corrected gettimeofday value for formated time
 * values.
 *
 * @param tv address of the struct timeval buffer.
 * @param timestamp value via sphgettimer().
 * @param tb2gtod_factor timebase to gettimeofday conversion factor.
 * @return The timebase converted to gettimeofday in struct timeval.
 */

extern __C__ int
sphtb2gtod_withfactor (struct timeval *tv,
		sphtimer_t timestamp,
		sphtimer_t tb2gtod_factor);

#endif /* __SPH_GTOD_H */
