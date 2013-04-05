#include "SOC.h"


#define ADC_GRP1_NUM_CHANNELS   3
#define ADC_GRP1_BUF_DEPTH      1

static adcsample_t samples1[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];//entspicht uin16_t
bool Akku_leer=TRUE;

static const ADCConversionGroup adcgrpcfg1 = {
  TRUE,
  ADC_GRP1_NUM_CHANNELS,
  NULL,
  NULL,
  0,                        /* CR1 */
  ADC_CR2_SWSTART,          /* CR2 */
  ADC_SMPR1_SMP_AN10(ADC_SAMPLE_3) | ADC_SMPR1_SMP_AN11(ADC_SAMPLE_3) | ADC_SMPR1_SMP_AN12(ADC_SAMPLE_3),
  0,                        /* SMPR2 */
  ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS),
  0,                        /* SQR2 */
  ADC_SQR3_SQ1_N(ADC_CHANNEL_IN10)| ADC_SQR3_SQ2_N(ADC_CHANNEL_IN11)| ADC_SQR3_SQ3_N(ADC_CHANNEL_IN12)
};


void setup_SOC()
{
	palSetGroupMode(GPIOC, PAL_PORT_BIT(0) | PAL_PORT_BIT(1) | PAL_PORT_BIT(2),0, PAL_MODE_INPUT_ANALOG);
	adcStart(&ADCD1,NULL);
	adcStartConversion(&ADCD1, &adcgrpcfg1, samples1, ADC_GRP1_BUF_DEPTH);
}

void check_SOC()
{
	short U_all, U_2S, U_1S;
	short U2,U3;
	/*Spannung aller Zellen
	normiert auf Wertebereich 0...3000
	0....		alle Zellen leer   	~9V 
	3000...	alle Zellen voll		~12,6V  */
	U_all=(samples1[0]-2758)*3000/1103;	
	
	/*Spannung von 2 Zellen
	normiert auf Wertebereich 0...2000
	0....		alle Zellen leer   	~6V 
	2000...	alle Zellen voll		~8,4V   */
	U_2S=(samples1[2]-2702)*2000/1076;
	
	/*Spannung der 1. Zelle
	normiert auf Wertebereich 0...1000
	0....		Zelle leer   	~3V 
	1000...	Zelle voll		~4,2V   */
	U_1S=(samples1[1]-2785)*1000/1117;
	
	/*Spannung der einzelnen Zellen ermitteln
	nun enthalten die drei Variablen U_1S, U2 und U3 jeweils die Spannung einer Zelle normiert auf 0...1000
	0....		Zelle leer ~3V
	1000...	Zelle voll ~4,2V
	*/
	U3=U_all-U_2S;
	U2=U_2S-U_1S;
	if (U_1S<50 || U2<50 || U3<50)
	{
		palSetPad(GPIOD, GPIOD_LED5);	//rot
		Akku_leer=TRUE;
	}
	else 
	{
		palClearPad(GPIOD, GPIOD_LED6);
		Akku_leer=FALSE;
	}
}