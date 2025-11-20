// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

// If you don't want Nodecraft server event logic to be present in server builds, set this to 0
// This will essentially make ANodecraftGameBase equivalent to a normal AGameModeBase
#ifndef NC_USE_NODECRAFT_ON_SERVER
#define NC_USE_NODECRAFT_ON_SERVER 1
#endif