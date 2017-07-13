/* HiFive1/FE310 includes */

#include "platform.h"
#include "encoding.h"
#include "plic/plic_driver.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define PWM_SCALE 0x4
#define PWM_FREQ  30000
#define PWM_DUTY  PWM_FREQ/2 // 50% duty cycle

// Instance data for the PLIC.
static plic_instance_t g_plic;

void handle_m_time_interrupt()
{
}

void handle_m_ext_interrupt()
{
    puts("External interrupt")
    plic_source int_num = PLIC_claim_interrupt(&g_plic);
    switch (int_num) {
    case 0:
        break;
    case INT_PWM1_BASE:
        PWM1_REG(PWM_CFG) &= ~PWM_CFG_CMP0IP;
        break;
    }
    PLIC_complete_interrupt(&g_plic, int_num);
}

void init_PWM()
{
  PLIC_init(&g_plic, PLIC_CTRL_ADDR, PLIC_NUM_INTERRUPTS, PLIC_NUM_PRIORITIES);
  PLIC_enable_interrupt(&g_plic, INT_PWM1_BASE);
  PLIC_set_threshold(&g_plic, 0);
  PLIC_set_priority(&g_plic, INT_PWM1_BASE, 1);

  /* Configure PWM  */
  PWM1_REG(PWM_CFG) = 0; // Clear the configuration register

  /* This is the real meat of things. PWM configuration register bits are 
   * documented in the SiFive E300 Platform Reference Manual:
   *
   * https://www.sifive.com/documentation/freedom-soc/freedom-e300-platform-reference-manual/
   */
  PWM1_REG(PWM_CFG) =
    (PWM_CFG_ENALWAYS) |
    (PWM_CFG_ZEROCMP)  |
    (PWM_CFG_STICKY)   |
    (PWM_SCALE);

  PWM1_REG(PWM_CMP0) = PWM_FREQ;
  PWM1_REG(PWM_CMP1) = PWM_DUTY;
  PWM1_REG(PWM_CMP2) = PWM_DUTY;
  PWM1_REG(PWM_CMP3) = PWM_DUTY;

  PWM1_REG(PWM_COUNT) = 0;

  /* Re-enable timers */
  set_csr(mie, MIP_MTIP);
  set_csr(mstatus, MSTATUS_MIE);
}

int main()
{
    init_PWM();

    puts("Setup complete");

    while (1) {};

    return 0;
}
