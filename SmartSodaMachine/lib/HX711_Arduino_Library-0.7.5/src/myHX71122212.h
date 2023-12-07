#include <Arduino.h>

class HX711
{
private:
    byte PD_SCK;
    byte DOUT;
    byte GAIN;
    long OFFSET = 0;
    float SCALE = 1;

public:
    uint8_t shiftInSlow(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
    {
        uint8_t value = 0;
        uint8_t i;

        for (i = 0; i < 8; ++i)
        {
            PORTD |= (1 << PORTD3);
            delayMicroseconds(1);
            if (bitOrder == LSBFIRST)
                value |= digitalRead(dataPin) << i;
            else
                value |= digitalRead(dataPin) << (7 - i);
            PORTD &= ~(1 << PORTD3);
            delayMicroseconds(1);
        }
        return value;
    }

    bool is_ready()
    {
        return (PIND & (1 << PIND2)) == LOW;
    }

    void begin(byte gain)
    {
        // pin 2 en entrée et 3 en sortie
        DDRD |= (1 << DDD3);

        set_gain(gain);
    }

    void set_gain(byte gain)
    {
        switch (gain)
        {
        case 128: // channel A, gain factor 128
            GAIN = 1;
            break;
        case 64: // channel A, gain factor 64
            GAIN = 3;
            break;
        case 32: // channel B, gain factor 32
            GAIN = 2;
            break;
        }
    }

    void set_scale(float scale)
    { // mettre scale à 1f
        SCALE = scale;
    }

    void tare(byte times)
    {
        double sum = read_average(times);
        set_offset(sum);
    }

    void set_offset(long offset)
    {
        OFFSET = offset;
    }

    long read_average(byte times)
    {
        long l_long_sum = 0;
        for (byte iBcl = 0; iBcl < times; iBcl++)
        {
            l_long_sum += read();
        }
        return l_long_sum / times;
    }

    void wait_ready(unsigned long delay_ms)
    {

        while (!is_ready())
        {

            delay(delay_ms);
        }
    }

    long read()
    {

                /*
                // Wait for the chip to become ready.
                wait_ready();*/

                // Define structures for reading data into.
                unsigned long value = 0;
                uint8_t data[3] = {0};
                uint8_t filler = 0x00;

        #if HAS_ATOMIC_BLOCK
                ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
                {

        #else

                noInterrupts();
        #endif

            // Pulse the clock pin 24 times to read the data.
            data[2] = shiftInSlow(DOUT, PD_SCK, MSBFIRST);
            data[1] = shiftInSlow(DOUT, PD_SCK, MSBFIRST);
            data[0] = shiftInSlow(DOUT, PD_SCK, MSBFIRST);

            // Set the channel and the gain factor for the next reading using the clock pin.
            for (int iBcl = 0; iBcl < GAIN; iBcl++)
            {
                PORTD |= (1 << PORTD3);

        #if ARCH_ESPRESSIF
                        delayMicroseconds(1);
        #endif
                        PORTD &= ~(1 << PORTD3);
        #if ARCH_ESPRESSIF
                        delayMicroseconds(1);
        #endif
                    }

        #if IS_FREE_RTOS
                    // End of critical section.
                    portEXIT_CRITICAL(&mux);

        #else
                // Enable interrupts again.
                interrupts();
        #endif

        // Replicate the most significant bit to pad out a 32-bit signed integer
        if (data[2] & 0x80)
        {
            filler = 0xFF;
        }
        else
        {
            filler = 0x00;
        }

        // Construct a 32-bit signed integer
        value = (static_cast<unsigned long>(filler) << 24 | static_cast<unsigned long>(data[2]) << 16 | static_cast<unsigned long>(data[1]) << 8 | static_cast<unsigned long>(data[0]));

        return static_cast<long>(value);
    }

    float get_units(byte times)
    {
        return get_value(times) / SCALE;
    }

    double get_value(byte times)
    {
        return read_average(times) - OFFSET;
    }
};