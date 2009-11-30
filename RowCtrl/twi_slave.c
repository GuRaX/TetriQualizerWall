

/* Address is shifted one to the left side so only use Bit0-6 */
#define SLAVE_ADR   0x01

extern 


void twi_init(void) {
    TWAR = (SLAVE_ADR<<1) | (1<<TWGCE);     /* set Slave Address */
}


