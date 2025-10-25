#pragma once
#include <stdint.h>

typedef struct
{
	uint32_t Type;
	uint32_t Pad;
	uint64_t PhysicalStart;
	uint64_t VirtualStart;
	uint64_t NumberOfPages;
	uint64_t Attribute;
} MemoryDescriptor;

typedef struct
{
	MemoryDescriptor *mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
} MemoryInfo;

typedef struct
{
	void *BaseAddress;
	uint64_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine;
	unsigned char strategy;
} GraphicsInfo;

typedef struct
{

	/**
	 * @brief Grafische informatie van het huidige apparaat, zoals schermgrootte, bytes per pixel, etc
	 *
	 */
	GraphicsInfo *graphics_info;

	/**
	 * @brief Fontfile dat word gebruikt in de bootloader
	 *
	 */
	void *font;

	/**
	 * @brief Een link naar de memorymap van het systeem
	 *
	 */
	MemoryInfo *memory_info;

	/**
	 * @brief Een link naar de RSDP tabel voor ACPI informatie
	 *
	 */
	void *rsdp;
} BootInfo;