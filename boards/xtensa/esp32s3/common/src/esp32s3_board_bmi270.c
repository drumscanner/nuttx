/****************************************************************************
 * boards/xtensa/esp32/common/src/esp32_bmp180.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdio.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/sensors/bmi270.h>
#include <nuttx/i2c/i2c_master.h>

#include "esp32s3_i2c.h"
#include "esp32s3_board_bmi270.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_bmi270_initialize
 *
 * Description:
 *   Initialize and register the BMP180 Pressure Sensor driver.
 *
 * Input Parameters:
 *   devno - The device number, used to build the device path as /dev/pressN
 *   busno - The I2C bus number
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ****************************************************************************/

#define BMI270_I2C_ADDRESS  (0x68)

int board_bmi270_initialize(int devno, int busno)
{
  struct i2c_master_s *i2c;
  int ret;

  sninfo("Initializing BMI270!\n");

  /* Initialize BMI270 */

  i2c = esp32s3_i2cbus_initialize(busno);

  if (i2c)
    {
#ifdef CONFIG_SENSORS_BMI270_I2C && CONFIG_SENSORS_BMI270_UORB
      ret = bmi270_register_uorb(devno, i2c, BMI270_I2C_ADDRESS);
#endif
#ifdef CONFIG_SENSORS_BMI270_I2C      
      char devpath[12];
      snprintf(devpath, sizeof(devpath), "/dev/accel%d", devno);
      ret = bmi270_register(devpath, i2c, BMI270_I2C_ADDRESS);
#endif      
      if (ret < 0)
        {
          snerr("ERROR: Error registering BMI270 in I2C%d\n", busno);
        }
    }
  else
    {
      snerr("ERROR: I2C bus initialization error.");
      ret = -ENODEV;
    }

  return ret;
}

