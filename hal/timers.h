#pragma once
#include "stm8s.h"
//#include "static_assert.h"

namespace Mcudrv
{
	namespace Timers
	{
		enum Channel
		{
			Ch1,
			Ch2,
			Ch3,
			Ch4,
			All_Ch
		};
	}
	namespace T1
	{
		using namespace Timers;
		typedef uint16_t Div;
//TODO: Implement TIM1_SR2
		enum Cfg
		{
//			---=== TIM1_CR1 ===---
			Default = 0,
			CEN = TIM1_CR1_CEN,			// Counter enable
			UDIS = TIM1_CR1_UDIS,		// A UEV is not generated, shadow registers keep their value (ARR, PSC).
										// The counter and the prescaler are re-initialized if the UG bit is set.
			URS = TIM1_CR1_URS,		// An update interrupt request is sent only when the counter reaches the
									// overflow/underflow.
			OPM = TIM1_CR1_OPM,		// One Pulse Mode. Counter stops counting at the next update event (clearing the CEN bit)
			DIR = TIM1_CR1_DIR,		//Direction 0 - Up, 1 - Down
			ARPE = TIM1_CR1_ARPE,	// TIM4_ARR register is buffered through a preload register	

//			---================---			

//		Counter must be in Stop Mode CEN = 0

			EdgeAlign = 0,				// The counter counts up or down depending on the direction bit (DIR)		

//		Encoder mode must be disable (SMS = 0) in center aligned mode	

			CenterAlign1 = 0x01 << 5u,	// The counter counts up and down alternately. Output compare interrupt 
										// flags of channels configured in output (CCiS = 00 in TIM1_CCMRiregisters) are set only when the 
										// counter is counting down.
			CenterAlign2 = 0x02 << 5u,	// The counter counts up and down alternately. Output compare interrupt 
										// flags of channels configured in output (CCiS = 00 in CCMRiregisters) are set only when the counter 
										// is counting up. 
			CenterAlign3 = 0x03 << 5u,	// The counter counts up and down alternately. Output compare interrupt 
										// flags of channels configured in output (CCiS = 00 in TIM1_CCMRiregisters) are set both when the 
										// counter is counting up and down
			
//			---=== TIM1_CR2 ===---

		// Master mode selection
			TrigReset = 0,				// The UG bit from the TIM1_EGR register is used as trigger output (TRGO). If the reset is 
										// generated by the trigger input (clock/trigger mode controller configured in reset mode), the signal on 
										// TRGO is delayed compared to the actual reset.
			TrigEnable = 0x01 << 12u,	// The counter enable signal is used as trigger output (TRGO). It is used to start several 
										// timers or the ADC to control a window in which a slave timer or the ADC is enabled. The counter 
										// enable signal is generated by a logic OR between the CEN control bit and the trigger input when 
										// configured in trigger gated mode. When the counter enable signal is controlled by the trigger input, 
										// there is a delay on TRGO, except if the master/slave mode is selected (see the MSM bit description 
										// in TIM1_SMCR register).
			TrigUpdateEv = 0x02 << 12u,	// The update event is selected as trigger output (TRGO)
			TrigMatch = 0x03 << 12u,		// The trigger output sends a positive pulse when the CC1IF flag is to 
										// be set (even if it was already high), as soon as a capture or a compare match occurs (TRGO).
			TrigComp1 = 0x04 << 12u,		// OC1REF signal is used as trigger output (TRGO)
			TrigComp2 = 0x05 << 12u,		// OC2REF signal is used as trigger output (TRGO)
			TrigComp3 = 0x06 << 12u,		// OC3REF signal is used as trigger output (TRGO)
			TrigComp4 = 0x07 << 12u,		// OC4REF signal is used as trigger output (TRGO)
//			---================---			
			COMS = TIM1_CR2_COMS << 10u,		// 0: When capture/compare control bits are preloaded (CCPC = 1), they are updated by setting the COMG bit. 
										// 1: When capture/compare control bits are preloaded (CCPC = 1), they are updated by setting the 
										// COMG bit or when an rising edge occurs on TRGI.
//		This bit acts only on channels with complementary outputs
			CCPC = TIM1_CR2_CCPC << 8u,		// 0: The CCiE, CCiNE, CCiP, and CCiNP bits in the TIM1_CCERiregisters and the OCiM bit in the TIM1_CCMRiregisters are not preloaded
										// 1: CCiE, CCiNE, CCiP, C CiNP and OCiM bits are preloaded, after having been written, they are 
										// updated only when COMG bit is set in the TIM1_EGR register.
		};

		enum SlaveModeCtrl
		{
//			Default = 0,
			MSM = TIM1_SMCR_MSM,
			//TODO: implement
		};

		enum Ints
		{
			IRQ_Update = TIM1_IER_UIE,
			IRQ_Ch1 = TIM1_IER_CC1IE,
			IRQ_Ch2 = TIM1_IER_CC2IE,
			IRQ_Ch3 = TIM1_IER_CC3IE,
			IRQ_Ch4 = TIM1_IER_CC4IE,
			IRQ_Com = TIM1_IER_COMIE,
			IRQ_Trig = TIM1_IER_TIE,
			IRQ_Break = TIM1_IER_BIE
		};
		
		enum Events
		{
			Ev_Update = TIM1_EGR_UG,
			Ev_Ch1 = TIM1_EGR_CC1G,
			Ev_Ch2 = TIM1_EGR_CC2G,
			Ev_Ch3 = TIM1_EGR_CC3G,
			Ev_Trig = TIM1_EGR_TG
		};

		enum ActiveLevel
		{
			ActiveHigh,
			ActiveLow
		};

		enum ChannelType
		{
			Output,
			Input,
			InputMap0 = Input,  //ICx is mapped on TI1FPx
			InputMap1,			//ICx is mapped on TI2FPx
			InputMap2			//ICx is mapped on TRC
		};

		enum ChannelCfgIn
		{
			In_NoFilter,
			In_Filt_n2 = 0x01 << 4u,			// 0001: fSAMPLING= fMASTER, N = 2
			In_Filt_n4 = 0x02 << 4u,			// 0010: fSAMPLING= fMASTER, N = 4
			In_Filt_n8 = 0x03 << 4u,			// 0011: fSAMPLING= fMASTER, N = 8 
			In_Filt_div2_n8 = 0x05 << 4u,		// 0101: fSAMPLING= fMASTER/2, N = 8
			In_Filt_div4_n8 = 0x07 << 4u,		// 0111: fSAMPLING= fMASTER/4, N = 8
			In_Filt_div8_n8 = 0x09 << 4u,		// 1001: fSAMPLING= fMASTER/8, N = 8
			In_Filt_div16_n8 = 0x0C << 4u,		// 1100: fSAMPLING= fMASTER/16, N = 8
			In_Filt_div32_n8 = 0x0F << 4u,		// 1111: fSAMPLING= fMASTER/32, N = 8
			//			---================---			
			In_NoPresc = 0,						// 00: no prescaler, capture is done each time an edge is detected on the capture input
			In_Presc_2ev = 0x01 << 2u,			// 01: Capture is done once every 2 events
			In_Presc_4ev = 0x02 << 2u,			// 10: Capture is done once every 4 events
			In_Presc_8ev = 0x03 << 2u,			// 11: Capture is done once every 8 events
		};

		enum ChannelCfgOut
		{
			OutCompareClear = 0x80,				// OCxCE This bit is used to enable the clearing of the channel x output compare signal (OCxREF) by an 
												// external event on the TIM1_ETR pin (see Section 17.5.9 on page 181).
												// 	0: OC1REF is not affected by the ETRF input signal (derived from the TIM1_ETR pin)
												// 	1: OC1REF is cleared as soon as a high level is detected on ETRF input signal (derived from the 
												// 	TIM1_ETR pin).
//			---================---			
			Out_Frozen = 0,						// 000: Frozen - The comparison between the output compare register TIMx_CCRx and the counter 
			Out_ActiveOnMatch = 0x01 << 4u,		// 001: Set channel x to active level on match. OC1REF signal is forced high when the counter 
												// TIMx_CNT matches the capture/compare register x (TIMx_CCRx).
			Out_InactiveOnMatch = 0x02 << 4u,	// 010: Set channel x to inactive level on match. OC1REF signal is forced low when the counter 
												// TIMx_CNT matches the capture/compare register x (TIMx_CCRx).
			Out_ToggleOnMatch = 0x03 << 4u,		// 011: Toggle - OC1REF toggles when TIMx_CNT=TIMx_CCRx
			Out_ForceInactive = 0x04 << 4u,		// 100: Force inactive level - OCxREF is forced low
			Out_ForceActive = 0x05 << 4u,		// 101: Force active level - OCxREF is forced high
			Out_PWM_Mode1 = 0x06 << 4u,			// 110: PWM mode 1 - In up-counting, channel x is active as long as TIMx_CNT< TIMx_CCRx. 
												// Otherwise, channel x is inactive. In down-counting,channel x is inactive (OCxREF = 0) as long as 
												// TIMx_CNT> TIMx_CCRx. Otherwise, channel x is active (OCxREF = 1).
			Out_PWM_Mode2 = 0x07 << 4u,			// 111: PWM mode 2 - In up-counting, channel x is inactive as long as TIMx_CNT< TIMx_CCRx. Otherwise, channel x is active
			//		---================---			
			Out_PreloadEnable = TIM1_CCMR_OCxPE,	// Preload register on TIMx_CCRx enabled. Read/write operations access the preload register. 
			//		---================---			
			Out_FastEnable = TIM1_CCMR_OCxFE		// This bit is used to accelerate the effect of an event on the trigger in input on the CC output.
													//	0: CC1 behaves normally depending on the counter and CCR1 values, even when the trigger is on. 
													//	The minimum delay to activate CC1 output when an edge occurs on the trigger input, is 5 clock 
													//	cycles.
													//	1: An active edge on the trigger input acts like a compare match on the CC1 output. If this happens, 
													//	OC is set to the compare level irrespective of the result of the comparison. The delay to sample the 
													//	trigger input and to activate CC1 output is reduced to 3 clock cycles. OCFE acts only if the channel 
													//	is configured in PWM1 or PWM2 mode. 
		};

		class Timer1
		{
		public:
			static void Init(const uint16_t divider, const Cfg config = Default)
			{
				TIM1->PSCRH = (divider - 1) >> 8UL;
				TIM1->PSCRL = (uint8_t)(divider - 1);
				TIM1->CR1 = (uint8_t)config;
				TIM1->CR2 = config >> 8u;
				TIM1->BKR |= TIM1_BKR_MOE;
			}

			FORCEINLINE
			static void Enable()
			{
				TIM1->CR1 |= TIM1_CR1_CEN;
			}
			FORCEINLINE
			static void Disable()
			{
				TIM1->CR1 &= ~TIM1_CR1_CEN;
			}
			FORCEINLINE
			static void EnableInterrupt(const Ints mask)
			{
				TIM1->IER |= mask;
			}
			FORCEINLINE
			static void DisableInterrupt(const Ints mask)
			{
				TIM1->IER &= ~mask;
			}
			FORCEINLINE
			static bool CheckIntStatus(const Ints flag)
			{
				return TIM1->SR1 & flag;
			}
			FORCEINLINE
			static void ClearIntFlag(const Ints flag)
			{
				TIM1->SR1 &= ~flag;
			}
			FORCEINLINE
			static void TriggerEvent(const Events ev)
			{
				TIM1->EGR |= ev;
			}
			
			FORCEINLINE
			static void WriteCounter(const uint16_t c)	//Need to stop Timer
			{
	//			Disable();
				TIM1->CNTRH = c >> 8;
				TIM1->CNTRL = (uint8_t)c;
	//			Enable();
			}

			#pragma diag_suppress=Pe940
			static uint16_t ReadCounter()
			{
				__asm("LD A, L:0x525e\n"
						"LD XH, A\n"
						"LD A, L:0x525f\n"
						"LD XL, A\n");
			}
			#pragma diag_default=Pe940
			
			FORCEINLINE
			static void WriteAutoReload(const uint16_t c)
			{
				TIM1->ARRH = c >> 8;
				TIM1->ARRL = (uint8_t)c;
			}

			FORCEINLINE
			template <Channel Ch, ChannelType type, ChannelCfgIn cfg>
			static void SetChannelCfg()
			{
				static_assert(type != Output, "error in __FUNC__");
				if(Ch == All_Ch)
				{
					TIM1->CCMR1 = TIM1->CCMR2 = TIM1->CCMR3 = TIM1->CCMR4 = static_cast<uint8_t>(type) | static_cast<uint8_t>(cfg);
				}
				else *reinterpret_cast<volatile uint8_t*>(&TIM1->CCMR1 + Ch) = static_cast<uint8_t>(type) | static_cast<uint8_t>(cfg);
			}

			FORCEINLINE
			template <Channel Ch, ChannelType type, ChannelCfgOut cfg>
			static void SetChannelCfg()
			{
				static_assert(type == Output, "error in __FUNC__");
				if(Ch == All_Ch)
				{
					TIM1->CCMR1 = TIM1->CCMR2 = TIM1->CCMR3 = static_cast<uint8_t>(type) | static_cast<uint8_t>(cfg);
				}
				else *reinterpret_cast<volatile uint8_t*>(&TIM1->CCMR1 + Ch) = static_cast<uint8_t>(type) | static_cast<uint8_t>(cfg);
			}
			
			FORCEINLINE
			template <Channel Ch, ActiveLevel level = ActiveHigh>
			static void ChannelEnable()
			{
				if(Ch == All_Ch)
				{
					TIM1->CCER2 |= TIM1_CCER2_CC3E | (level << 1) | TIM1_CCER2_CC4E | ((level << 1) << 4);
					TIM1->CCER1 |= TIM1_CCER1_CC1E | (level << 1) | TIM1_CCER1_CC2E | ((level << 1) << 4);
				}
				if (Ch == Ch4) TIM1->CCER2 |= TIM1_CCER2_CC4E | ((level << 1) << 4);
				if (Ch == Ch3) TIM1->CCER2 |= TIM1_CCER2_CC3E | (level << 1);
				if (Ch == Ch2) TIM1->CCER1 |= TIM1_CCER1_CC2E | ((level << 1) << 4);
				if (Ch == Ch1) TIM1->CCER1 |= TIM1_CCER1_CC1E | (level << 1);
			}

			FORCEINLINE
			template <Channel Ch>
			static void ChannelDisable()
			{
				static_assert(Ch != All_Ch, "error in __FUNC__");
				if (Ch & Ch4 == Ch4) TIM1->CCER2 &= ~(0x0F << 4);
				if (Ch & Ch3 == Ch3) TIM1->CCER2 &= ~0x0F;
				if (Ch & Ch2 == Ch2) TIM1->CCER1 &= ~(0x0F << 4);
				if (Ch & Ch1 == Ch1) TIM1->CCER1 &= ~0x0F;
			}
			
			FORCEINLINE
			template <Channel Ch, ActiveLevel level = ActiveHigh>
			static void ChannelEnableComplementary()
			{
				static_assert(Ch == Ch4, "error in __FUNC__");
				if (Ch & Ch3 == Ch3) TIM1->CCER2 |= TIM1_CCER2_CC3NE | (level << 3);
				if (Ch & Ch2 == Ch2) TIM1->CCER1 |= TIM1_CCER1_CC2NE | ((level << 3) << 4);
				if (Ch & Ch1 == Ch1) TIM1->CCER1 |= TIM1_CCER1_CC1NE | (level << 3);
			}
 		
			FORCEINLINE
			template<Channel Ch>
			static void WriteCompareWord(const uint16_t c)
			{
				*reinterpret_cast<volatile uint8_t*>(&TIM1->CCR1H + Ch * 2) = c >> 8;
				*reinterpret_cast<volatile uint8_t*>(&TIM1->CCR1L + Ch * 2) = c;
			}

			FORCEINLINE
			template<Channel Ch>
			static void WriteCompareByte(const uint8_t c)
			{
				*reinterpret_cast<volatile uint8_t*>(&TIM1->CCR1L + Ch * 2) = c;
			}

			FORCEINLINE
			template<Channel Ch>
			static uint16_t ReadCompareWord()
			{
				return *reinterpret_cast<volatile uint16_t*>(&TIM1->CCR1H + Ch * 2);
			}

			FORCEINLINE
			template<Channel Ch>
			static uint8_t ReadCompareByte()
			{
				return *reinterpret_cast<volatile uint8_t*>(&TIM1->CCR1L + Ch * 2);
			}

			FORCEINLINE
			template<Channel Ch>
			static volatile uint16_t& GetCompareWord()
			{
				return *reinterpret_cast<volatile uint16_t*>(&TIM1->CCR1H + Ch * 2);
			}

			FORCEINLINE
			template<Channel Ch>
			static volatile uint8_t& GetCompareByte()
			{
				return *reinterpret_cast<volatile uint8_t*>(&TIM1->CCR1L + Ch * 2);
			}
		}; //Timer1

	} //T1

	namespace T2
	{
		using namespace Timers;
		enum Div
			{
				Div_1,
				Div_2,
				Div_4,
				Div_8,
				Div_16,
				Div_32,
				Div_64,
				Div_128,
				Div_256,
				Div_512,
				Div_1024,
				Div_2048,
				Div_4096,
				Div_8192,
				Div_16384,
				Div_32768,
			};
		enum Cfg
			{
				Default = 0,
				CEN = TIM2_CR1_CEN,		// Counter enable
				UDIS = TIM2_CR1_UDIS,	// A UEV is not generated, shadow registers keep their value (ARR, PSC).
															// The counter and the prescaler are re-initialized if the UG bit is set.
				URS = TIM2_CR1_URS,		// An update interrupt request is sent only when the counter reaches the
															// overflow/underflow.
				OPM = TIM2_CR1_OPM,		// One Pulse Mode
				ARPE = TIM2_CR1_ARPE  // TIM2_ARR register is buffered through a preload register
			};
		enum Ints
			{
				IRQ_Update = TIM2_IER_UIE,
				IRQ_Ch1 = TIM2_IER_CC1IE,
				IRQ_Ch2 = TIM2_IER_CC2IE,
				IRQ_Ch3 = TIM2_IER_CC3IE
			};
		enum Events
		{
			Ev_Update = TIM2_EGR_UG,
			Ev_Ch1 = TIM2_EGR_CC1G,
			Ev_Ch2 = TIM2_EGR_CC2G,
			Ev_Ch3 = TIM2_EGR_CC3G
		};
		enum ChannelType
		{
			Output,
			Input,
			InputMap0 = Input,  //ICx is mapped on TI1FPx
			InputMap1,  //ICx is mapped on TI2FPx
		};
		enum ActiveLevel
			{
				ActiveHigh,
				ActiveLow
			};
		enum ChannelCfgIn
		{
			In_NoFilter,
			In_Filt_n2 = 0x01 << 4u,			// 0001: fSAMPLING= fMASTER, N = 2
			In_Filt_n4 = 0x02 << 4u,			// 0010: fSAMPLING= fMASTER, N = 4
			In_Filt_n8 = 0x03 << 4u,			// 0011: fSAMPLING= fMASTER, N = 8 
			In_Filt_div2_n8 = 0x05 << 4u,		// 0101: fSAMPLING= fMASTER/2, N = 8
			In_Filt_div4_n8 = 0x07 << 4u,		// 0111: fSAMPLING= fMASTER/4, N = 8
			In_Filt_div8_n8 = 0x09 << 4u,		// 1001: fSAMPLING= fMASTER/8, N = 8
			In_Filt_div16_n8 = 0x0C << 4u,		// 1100: fSAMPLING= fMASTER/16, N = 8
			In_Filt_div32_n8 = 0x0F << 4u,		// 1111: fSAMPLING= fMASTER/32, N = 8
//			---================---			
			In_NoPresc = 0,						// 00: no prescaler, capture is done each time an edge is detected on the capture input
			In_Presc_2ev = 0x01 << 2u,			// 01: Capture is done once every 2 events
			In_Presc_4ev = 0x02 << 2u,			// 10: Capture is done once every 4 events
			In_Presc_8ev = 0x03 << 2u,			// 11: Capture is done once every 8 events
		};
		enum ChannelCfgOut
		{
			Out_Frozen = 0,						// 000: Frozen - The comparison between the output compare register TIMx_CCRx and the counter 
			Out_ActiveOnMatch = 0x01 << 4u,		// 001: Set channel x to active level on match. OC1REF signal is forced high when the counter 
												// TIMx_CNT matches the capture/compare register x (TIMx_CCRx).
			Out_InactiveOnMatch = 0x02 << 4u,	// 010: Set channel x to inactive level on match. OC1REF signal is forced low when the counter 
												// TIMx_CNT matches the capture/compare register x (TIMx_CCRx).
			Out_ToggleOnMatch = 0x03 << 4u,		// 011: Toggle - OC1REF toggles when TIMx_CNT=TIMx_CCRx
			Out_ForceInactive = 0x04 << 4u,		// 100: Force inactive level - OCxREF is forced low
			Out_ForceActive = 0x05 << 4u,		// 101: Force active level - OCxREF is forced high
			Out_PWM_Mode1 = 0x06 << 4u,			// 110: PWM mode 1 - In up-counting, channel x is active as long as TIMx_CNT< TIMx_CCRx. 
												// Otherwise, channel x is inactive. In down-counting,channel x is inactive (OCxREF = 0) as long as 
												// TIMx_CNT> TIMx_CCRx. Otherwise, channel x is active (OCxREF = 1).
			Out_PWM_Mode2 = 0x07 << 4u,			// 111: PWM mode 2 - In up-counting, channel x is inactive as long as TIMx_CNT< TIMx_CCRx. Otherwise, channel x is active
		//		---================---			
			Out_PreloadEnable = 0x01 << 3u,			// Preload register on TIMx_CCRx enabled. Read/write operations access the preload register. 
														// TIMx_CCRx preload value is loaded in the shadow register at each update event.
		};

		namespace Internal
		{
			template<uint16_t>
			struct TypeOfTimer;
			template<>
			struct TypeOfTimer<TIM2_BaseAddress>
			{
				typedef TIM2_TypeDef type;
			};
			template<>
			struct TypeOfTimer<TIM3_BaseAddress>
			{
				typedef TIM3_TypeDef type;
			};

			template<uint16_t BaseAddr>
			class Timer
			{
			private:
				typedef typename TypeOfTimer<BaseAddr>::type TIM_TypeDef;

				FORCEINLINE
				static TIM_TypeDef* Regs()
				{
					return reinterpret_cast<TIM_TypeDef*>(BaseAddr);
				}
			public:
				static void Init(const Div divider, const Cfg config)
				{
					Regs()->PSCR = divider;
					Regs()->CR1 = config;
				}

				FORCEINLINE
				static void Enable()
				{
					Regs()->CR1 |= TIM2_CR1_CEN;
				}
				FORCEINLINE
				static void Disable()
				{
					Regs()->CR1 &= ~TIM2_CR1_CEN;
				}
				FORCEINLINE
				static void EnableInterrupt(const Ints mask)
				{
					Regs()->IER |= mask;
				}
				FORCEINLINE
				static void DisableInterrupt(const Ints mask)
				{
					Regs()->IER &= ~mask;
				}
				FORCEINLINE
				static void TriggerEvent(const Events ev)
				{
					Regs()->EGR |= ev;
				}
				FORCEINLINE
				static bool CheckIntStatus(const Ints flag)
				{
					return Regs()->SR1 & flag;
				}
				FORCEINLINE
				static void ClearIntFlag(const Ints flag)
				{
					Regs()->SR1 &= ~flag;
				}
				static void WriteCounter(const uint16_t c)	//Need to stop Timer
				{
		//			Disable();
					Regs()->CNTRH = c >> 8;
					Regs()->CNTRL = c;
		//			Enable();
				}
				static uint16_t ReadCounter()
				{
					uint8_t msb = Regs()->CNTRH;
					uint8_t lsb = Regs()->CNTRL;
					return lsb | (msb << 8);
				}
				static void WriteAutoReload(const uint16_t c)
				{
					Regs()->ARRH = c >> 8;
					Regs()->ARRL = c;
				}

				FORCEINLINE
				template <Channel Ch, ChannelType type, ChannelCfgIn cfg>
				static void SetChannelCfg()
				{
					static_assert(!(Ch == Ch3 && BaseAddr == TIM3_BaseAddress), "Timer 3 have no Channel 3");
					static_assert(type != Output, "error in __FUNC__");
					if(Ch == All_Ch)
					{
						Regs()->CCMR1 = Regs()->CCMR2 = static_cast<uint8_t>(type) | static_cast<uint8_t>(cfg);
						if(BaseAddr == TIM2_BaseAddress) TIM2->CCMR3 = static_cast<uint8_t>(type) | static_cast<uint8_t>(cfg);
					}
					else *reinterpret_cast<volatile uint8_t*>(&Regs()->CCMR1 + Ch) = static_cast<uint8_t>(type) | static_cast<uint8_t>(cfg);
				}

				FORCEINLINE
				template <Channel Ch, ChannelType type, ChannelCfgOut cfg>
				static void SetChannelCfg()
				{
					static_assert(!(Ch == Ch3 && BaseAddr == TIM3_BaseAddress), "Timer 3 have no Channel 3");
					static_assert(type == Output, "error in __FUNC__");
					if(Ch == All_Ch)
					{
						Regs()->CCMR1 = Regs()->CCMR2 = static_cast<uint8_t>(type) | static_cast<uint8_t>(cfg);
						if(BaseAddr == TIM2_BaseAddress) TIM2->CCMR3 = static_cast<uint8_t>(type) | static_cast<uint8_t>(cfg);
					}
					else *reinterpret_cast<volatile uint8_t*>(&Regs()->CCMR1 + Ch) = static_cast<uint8_t>(type) | static_cast<uint8_t>(cfg);
				}

				FORCEINLINE
				template <Channel Ch, ActiveLevel level = ActiveHigh>
				static void ChannelEnable()
				{
					static_assert(!(Ch == Ch3 && BaseAddr == TIM3_BaseAddress), "Timer 3 have no Channel 3");
					if(Ch == All_Ch)
					{
						if(BaseAddr == TIM2_BaseAddress) TIM2->CCER2 |= TIM2_CCER2_CC3E | (level << 1);
						Regs()->CCER1 |= TIM2_CCER1_CC1E | (level << 1) | TIM2_CCER1_CC2E | ((level << 1) << 4);
					}
					if (Ch == Ch3) TIM2->CCER2 |= TIM2_CCER2_CC3E | (level << 1);
					if (Ch == Ch2) Regs()->CCER1 |= TIM2_CCER1_CC2E | ((level << 1) << 4);
					if (Ch == Ch1) Regs()->CCER1 |= TIM2_CCER1_CC1E | (level << 1);
				}

				FORCEINLINE
				template <Channel Ch>
				static void ChannelDisable()
				{
					static_assert(!(Ch == Ch3 && BaseAddr == TIM3_BaseAddress), "Timer 3 have no Channel 3");
					if(Ch == All_Ch)
					{
						Regs()->CCER1 = 0;
						if(BaseAddr == TIM2_BaseAddress) TIM2->CCER2 = 0;
					}
					if (Ch == Ch3) TIM2->CCER2 &= ~0x0F;
					if (Ch == Ch2) Regs()->CCER1 &= ~(0x0F << 4);
					if (Ch == Ch1) Regs()->CCER1 &= ~0x0F;
				}

				FORCEINLINE
				template<Channel Ch>
				static void WriteCompareWord(const uint16_t c)
				{
					static_assert(!(Ch == Ch3 && BaseAddr == TIM3_BaseAddress), "Timer 3 have no Channel 3");
					*reinterpret_cast<volatile uint8_t*>(&Regs()->CCR1H + Ch * 2) = c >> 8;
					*reinterpret_cast<volatile uint8_t*>(&Regs()->CCR1L + Ch * 2) = c;
				}

				FORCEINLINE
				template<Channel Ch>
				static void WriteCompareByte(const uint8_t c)
				{
					static_assert(!(Ch == Ch3 && BaseAddr == TIM3_BaseAddress), "Timer 3 have no Channel 3");
					*reinterpret_cast<volatile uint8_t*>(&Regs()->CCR1L + Ch * 2) = c;
				}

				FORCEINLINE
				template<Channel Ch>
				static uint16_t ReadCompareWord()
				{
					static_assert(!(Ch == Ch3 && BaseAddr == TIM3_BaseAddress), "Timer 3 have no Channel 3");
					return *reinterpret_cast<volatile uint16_t*>(&Regs()->CCR1H + Ch * 2);
				}

				FORCEINLINE
				template<Channel Ch>
				static uint8_t ReadCompareByte()
				{
					static_assert(!(Ch == Ch3 && BaseAddr == TIM3_BaseAddress), "Timer 3 have no Channel 3");
					return *reinterpret_cast<volatile uint8_t*>(&Regs()->CCR1L + Ch * 2);
				}

				FORCEINLINE
				template<Channel Ch>
				static volatile uint16_t& GetCompareWord()
				{
					static_assert(!(Ch == Ch3 && BaseAddr == TIM3_BaseAddress), "Timer 3 have no Channel 3");
					return *reinterpret_cast<volatile uint16_t*>(&Regs()->CCR1H + Ch * 2);
				}

				FORCEINLINE
				template<Channel Ch>
				static volatile uint8_t& GetCompareByte()
				{
					static_assert(!(Ch == Ch3 && BaseAddr == TIM3_BaseAddress), "Timer 3 have no Channel 3");
					return *reinterpret_cast<volatile uint8_t*>(&Regs()->CCR1L + Ch * 2);
				}
			}; //Timer2-3
		}//Internal
		typedef Internal::Timer<TIM2_BaseAddress> Timer2;
		typedef Internal::Timer<TIM3_BaseAddress> Timer3;
	} //T2
	namespace T3 = T2;

	namespace T4
	{
			enum Div
			{
				Div_1,
				Div_2,
				Div_4,
				Div_8,
				Div_16,
				Div_32,
				Div_64,
				Div_128
			};

			enum Cfg
			{
				CEN = TIM4_CR1_CEN,		// CEN Counter enable
				UDIS = TIM4_CR1_UDIS,	// UDIS  A UEV is not generated, shadow registers keep their value (ARR, PSC).
										//The counter and the	prescaler are re-initialized if the UG bit is set.
				URS = TIM4_CR1_URS,		// URS an update interrupt request is sent only when the counter reaches the
										//overflow/underflow.
				OPM = TIM4_CR1_OPM,
				ARPE = TIM4_CR1_ARPE	//ARPE TIM4_ARR register is buffered through a preload register
			};

		class Timer4
		{
		public:
			FORCEINLINE
			static void Init(const Div Divider, const Cfg Config)
			{
				TIM4->PSCR = Divider;
				TIM4->CR1 = Config;
			}
			FORCEINLINE
			static void Enable()
			{
				TIM4->CR1 |= TIM4_CR1_CEN;
			}
			FORCEINLINE
			static void Disable()
			{
				TIM4->CR1 &= ~TIM4_CR1_CEN;
			}
			FORCEINLINE
			static void ResetCounter()
			{
				TIM4->CNTR = 0;
			}
			FORCEINLINE
			static void EnableInterrupt() // Only one interrupt on that vector (Update)
			{
				TIM4->IER = TIM4_IER_UIE;
			}
			FORCEINLINE
			static void DisableInterrupt()
			{
				TIM4->IER = 0;
			}
			FORCEINLINE
			static bool CheckIntStatus()		
			{
				return TIM4->SR1;
			}
			FORCEINLINE
			static void ClearIntFlag()
			{
				TIM4->SR1 = 0;
			}
			FORCEINLINE
			static void WriteCounter(uint8_t c)
			{
				TIM4->CNTR = c;
			}
			FORCEINLINE
			static uint8_t ReadCounter()
			{
				return TIM4->CNTR;
			}
			static void WriteAutoReload(uint8_t c)
			{
				TIM4->ARR = c;
			}
		};
	} //T4

	namespace Wdg
	{
		enum Period
		{
			P_16ms,
			P_32ms,
			P_64ms,
			P_128ms,
			P_256ms,
			P_512ms,
			P_1s
		};
		
		class Iwdg
		{
		public:
			FORCEINLINE
			static void Enable(const Period period)
			{
				IWDG->KR = 0xCC;
				IWDG->KR = 0x55;
				IWDG->RLR = 0xFF;
				IWDG->PR = period;
			}
			FORCEINLINE
			static void Refresh()
			{
				IWDG->KR = 0xAA;
			}
		};
	}
}
