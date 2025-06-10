#ifndef MODULELOADER_H
#define MODULELOADER_H

/**
 * @brief Loads multiple binary modules from a payload into memory.
 * 
 * @param payloadStart Pointer to the beginning of the payload containing modules.
 * @param moduleTargetAddress Array of pointers specifying where each module should be loaded.
 */
void loadModules(void * payloadStart, void ** moduleTargetAddress);

#endif