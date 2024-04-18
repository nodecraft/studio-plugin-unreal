// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

// NOTE: Please append either SUCCESS or FAILURE to the end of the message code to indicate the result of the event.
// This will help with finding / discovering message codes via autocomplete

// Game Server Events
#define GAME_SERVER_FINISHED_RECONNECTION_PHASE_SUCCESS "game_server.finished_reconnection_phase.success"

// Game Server Generic
#define GAME_SERVER_INVALID_TOKEN_FAILURE "game_server.invalid_token"

// Ident
#define IDENT_MISSING_TOKEN_FAILURE "ident.missing_token"
#define IDENT_INVALID_CODE_FAILURE "ident.invalid_code"
#define IDENT_INVALID_TYPE_FAILURE "ident.invalid_type"
#define IDENT_INCOMPATIBLE_TYPE_FAILURE "ident.incompatible_type"
#define IDENT_MISSING_IP_FAILURE "ident.missing_ip"
#define IDENT_PLATFORM_FAILURE "ident.platform_error"
#define IDENT_PLATFORM_CONFIG_FAILURE "ident.platform_config_error"
#define IDENT_MISSING_LEGAL_CONSENT_FAILURE "ident.missing_legal_consent"
#define IDENT_MISSING_RULES_CONSENT_FAILURE "ident.missing_rules_consent"
#define IDENT_CHALLENGE_REQUESTED_SUCCESS "ident.challenge_requested"
#define IDENT_SUCCESS "ident.success"
#define IDENT_SESSION_SUCCESS "ident.session_success"
#define IDENT_SESSION_UPDATE_SUCCESS "ident.session.update.success"
#define IDENT_GAME_IDENTS_SUCCESS "ident.game_idents"

// Server
#define SERVER_VALIDATION_FAILURE "server.validation"

// Allows
#define ALLOWS_UPDATE_SUCCESS "allows.update"
#define ALLOWS_UPDATE_STATUS_MISMATCH_FAILURE "allows.update.status_mismatch"
#define ALLOWS_UPDATE_FAILED "allows.update.failed"

// Invites
#define INVITES_UPDATE_SUCCESS "invites.update.success"
#define INVITES_UPDATE_STATUS_MISMATCH_FAILURE "invites.update.status_mismatch"
#define INVITES_UPDATE_FAILED "invites.update.failed"

// Notifications
#define NOTIFICATIONS_DISMISSED_SUCCESS "notifications.dismissed"
#define NOTIFICATIONS_DISMISSED_MULTI_SUCCESS "notifications.dismissed_multi"

// Reputation
#define REPUTATION_WILDCARD "reputation.*"

// Server Discovery
#define DISCOVERY_SERVER_WILDCARD "discovery.server.*"

// Player Connection
#define PLAYER_CONNECTION_GET_SUCCESS "player_connections.get.success"
#define PLAYER_CONNECTION_GET_NOT_FOUND_FAILURE "player_connections.get.not_found"
#define PLAYER_CONNECTION_CHALLENGE_CONSUME_SUCCESS "player_connections.challenge_consume.success"
#define PLAYER_CONNECTION_CHALLENGE_CONSUME_FAILURE "player_connections.challenge_consume.failed"

// Generic
#define PLAYER_ACCESS_TOKEN_WILDCARD "player_access_token.*"
#define PLAYER_ACCESS_TOKEN_INVALID_TOKEN_FAILURE "player_access_token.invalid_token"
#define GENERIC_WILDCARD_FAILURE "generic.*.failure"