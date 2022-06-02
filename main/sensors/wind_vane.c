#include "./wind_vane.h"
#include "driver/adc.h"

#define SAMPLE_CNT 30

static const adc1_channel_t adc_channel = ADC_WIND_VANE_PIN;

static void init_wind_vane(void){
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(adc_channel, ADC_ATTEN_DB_11);
}

char *getWindDirection(){
    init_wind_vane();

    char* wind_direction = "0";
    char* wind_cardinal_direction = "N";


    // The instructions provide approximate values for a voltage divider based on a 5V Arduino. 
    // These days there are probably more 3.3V devices out there and the values are different. 
    // I found it was better to simply work off of the analog values after sampling at every possible direction (
    // N, NNW, NW, WNW, W........). Takes a bit of time, but then a simple table allows a more reliable reading. 
    // I've included mine (using a 10k resistor. The value read must be LESS THAN the table value to represent the direction. 
    // Hope this is useful to someone. struct wind_Dir_Def { char dir_ASC[4]; uint16_t dir_Val;
    // uint16_t dir_Cnt;
    // }; // Uses a 10K resistor for VD wind_Dir_Def windDirTable3V[16] = 
    // { {"ESE",84,0}, {"ENE",92,0}, {"E",126,0}, {"SSE",185,0}, {"SE",245,0}, {"SSW",287,0}, 
    // {"S",408,0}, {"NNE",463,0}, {"NE",599,0}, {"WSW",629,0}, {"SW",702,0}, {"NNW",784,0}, 
    // {"N",824,0}, {"WNW",883,0}, {"NW",939,0}, {"W",1024,0}}; 
    // // Check all the possible values to determine the closest to what we have 
    // for(int ix = 0; ix < numDirs; ix++) { if(rawWindDir < windDirTable3V[ix].dir_Val) 
    // { // We tally up the number of hits for a particular direction and then use the one with the most 
    // for reporting windDirTable3V[ix].dir_Cnt++; prevWindIdx = ix; break; } }

    //calculate with voltage divider from documentation. finalize this when the circuit is in place
    int analog_voltage_map[15] = {150, 300, 450, 600, 830, 1100, 1500, 1700, 2250, 2350, 2700, 3000, 3200, 3400, 3900};
    
    //update this to use weatherbit specifications
    char* wind_dir_map[15] = {"157.5", "180", "247.5", "202.5", "225", "270", "292.5", "112.5", "135", "337.5", "315", "67.5", "90", "22.5", "45"};
    char* wind_cardinal_map[15] = {"SSE", "S", "WSW", "SSW", "SW", "W", "WNW", "ESE", "SE", "NNW", "NW", "ENE", "E", "NNE", "NE"};

    uint32_t adc_val = 0;
    for (int i = 0; i < SAMPLE_CNT; ++i)
    {
        adc_val += adc1_get_raw(adc_channel);
    }

    uint32_t vin = adc_val / SAMPLE_CNT; //average reading

    for (int i = 0; i < 15; i++)
    {
        if (vin < analog_voltage_map[i])
        {
            wind_direction = wind_dir_map[i];
            wind_cardinal_direction = wind_cardinal_map[i];
            break;
        }
    }
    printf("Analog value: %i Wind direction: %s, %s  \n", vin, wind_direction, wind_cardinal_direction);
   
    return wind_cardinal_direction;
}