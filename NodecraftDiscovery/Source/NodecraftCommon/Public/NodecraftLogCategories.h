// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Logging/LogMacros.h"

// Logs related to the Nodecraft authentication system.
// Turn on Verbose logging for more info.
DEFINE_LOG_CATEGORY_STATIC(LogNodecraftAuth, Log, All);

// Logs related to HTTP requests and responses
DEFINE_LOG_CATEGORY_STATIC(LogNodecraftHTTP, NoLogging, All);

// Logs related to the events specific to game server events.
// Turn on Verbose logging for more info.
DEFINE_LOG_CATEGORY_STATIC(LogNodecraftGameServerEvents, Log, All);

// Logs related to the Nodecraft UI system.
DEFINE_LOG_CATEGORY_STATIC(LogNodecraftUI, Log, All);

// Additional logs that don't fit into the other categories.
// Mostly used for error reporting and debugging purposes.
// If you are running into issues with the plugin, turn on Verbose logging for more info.
DEFINE_LOG_CATEGORY_STATIC(LogNodecraftMisc, NoLogging, All);

// Logs related to the JSON parsing and serialization of the Nodecraft API.
// Turn on Verbose logging for more info.
DEFINE_LOG_CATEGORY_STATIC(LogNodecraftJson, NoLogging, All);

// Logs all messages that are sent from the message router.
// Mostly this will report all internal message codes and http codes of all HTTP calls
// made to the Nodecraft API (when running in development), but not solely.
// Verbose logging will tell you when message receivers are added or removed.
// If you want detailed information on the message responses of every HTTP message (along with a couple other things),
// turn this on to Very Verbose.
DEFINE_LOG_CATEGORY_STATIC(LogNodecraftMessageRouter, NoLogging, All);

///////////////////////////////////////////////////////////////////////
// Categories related to particular systems in the Nodecraft plugin. //
///////////////////////////////////////////////////////////////////////
DEFINE_LOG_CATEGORY_STATIC(LogNodecraft_Friends, NoLogging, All);
DEFINE_LOG_CATEGORY_STATIC(LogNodecraft_Moderation, NoLogging, All);
DEFINE_LOG_CATEGORY_STATIC(LogNodecraft_QrCodeLinks, NoLogging, All);
DEFINE_LOG_CATEGORY_STATIC(LogNodecraft_Servers, NoLogging, All);
DEFINE_LOG_CATEGORY_STATIC(LogNodecraft_RemoteImageSubsystem, NoLogging, All);


// TODO: Place log categories for all services here