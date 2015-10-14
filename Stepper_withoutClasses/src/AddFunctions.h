/*
 * AddFunctions.h
 *
 * Created: 09.08.2015 11:45:55
 *  Author: Roland-User
 */ 


#ifndef ADDFUNCTIONS_H_
#define ADDFUNCTIONS_H_

#include <asf.h>
#include <avr/io.h>

inline uint8_t NOP(void)
{
	return 0x0;
}

inline uint32_t SetParam(uint8_t PARAM, uint8_t VALUE3, uint8_t VALUE2, uint8_t VALUE1)
{
	return (((uint32_t)PARAM << 24) | ((uint32_t)VALUE3 << 16) | ((uint32_t)VALUE2 << 8) | (uint32_t)(VALUE1));
}

inline uint8_t GetParam(uint8_t PARAM)
{
	return ((uint8_t)0x20 | PARAM);
}

inline uint32_t Run(uint8_t DIR, uint8_t SPD3, uint8_t SPD2, uint8_t SPD1)
{
	return (((uint32_t)0x50 | DIR) << 24 | (uint32_t)SPD3 << 16 | (uint32_t)SPD2 << 8 | (uint32_t)SPD1);
}

inline uint8_t StepClock(uint8_t DIR)
{
	return ((uint8_t)0x58 | DIR);
}

inline uint32_t Move(uint8_t DIR, uint8_t N_STEP3, uint8_t N_STEP2, uint8_t N_STEP1)
{
	return (((uint32_t)0x40 | DIR) << 24 | ((uint32_t)N_STEP3 << 16) | ((uint32_t)N_STEP2 << 8) | N_STEP1);
}

inline uint32_t GoTo(uint8_t ABS_POS3, uint8_t ABS_POS2, uint8_t ABS_POS1)
{
	return ((uint32_t)0x60 << 24 | (uint32_t)ABS_POS3 << 16 | (uint32_t)ABS_POS2 << 8 | (uint32_t)ABS_POS1);
}

inline uint32_t GoTo_DIR(uint8_t DIR, uint8_t ABS_POS3, uint8_t ABS_POS2, uint8_t ABS_POS1)
{
	return ((uint32_t)((uint8_t)0x68 | DIR) << 24 | (uint32_t)ABS_POS3 << 16 | (uint32_t)ABS_POS2 << 8 | (uint32_t)ABS_POS1);
}

inline uint32_t GoUntil(uint8_t ACT, uint8_t DIR, uint8_t SPD3, uint8_t SPD2, uint8_t SPD1)
{
	return ((uint32_t)(((uint32_t)0x82 | (ACT << 4)) | DIR) << 24 | (uint32_t)SPD3 << 16 | SPD2 << 8 | SPD1);
}

inline uint8_t GoHome(void)
{
	return (uint8_t)0x70;
}

inline uint8_t GoMark(void)
{
	return (uint8_t)0x78;
}

inline uint8_t ResetPos(void)
{
	return (uint8_t)0xD8;
}

inline uint8_t ResetDevice(void)
{
	return (uint8_t)0xC0;
}

inline uint8_t SoftStop(void)
{
	return (uint8_t)0xB0;
}

inline uint8_t HardStop(void)
{
	return (uint8_t)0xB8;
}

inline uint8_t SoftHiZ(void)
{
	return (uint8_t)0xA0;
}

inline uint8_t HardHiZ(void)
{
	return (uint8_t)0xA8;
}

inline uint8_t GetStatus(void)
{
	return (uint8_t)0xD0;
}


#endif /* ADDFUNCTIONS_H_ */