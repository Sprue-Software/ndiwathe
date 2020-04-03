/**************************************************************************//**
 * Copyright 2018, Silicon Laboratories Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#ifndef SL_WFX_CONFIGURATION_H
#define SL_WFX_CONFIGURATION_H

#ifdef SL_WFX_SDIO_BLOCK_SIZE //Used in SDIO Block mode to round up the memory allocation to the block size used. In most case, SL_WFX_ROUND_UP_VALUE = WF200_SDIO_BLOCK_SIZE;
#define SL_WFX_ROUND_UP_VALUE                SL_WFX_SDIO_BLOCK_SIZE
#else
#define SL_WFX_ROUND_UP_VALUE                1
#endif

#ifndef SL_WFX_DEFAULT_REQUEST_TIMEOUT_MS
#define SL_WFX_DEFAULT_REQUEST_TIMEOUT_MS 5000  // Timeout period in milliseconds
#endif

#endif // SL_WFX_CONFIGURATION_H
