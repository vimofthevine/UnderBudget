package com.vimofthevine.underbudget.ledger

import io.ktor.locations.*

import java.util.UUID

@Location("/ledgers")
class LedgerResources()

@Location("/ledgers/{ledgerId}")
data class LedgerResource(val ledgerId: UUID)

@Location("/ledger-permissions")
class LedgerPermissionResources()

@Location("/ledger-permissions/{permissionId}")
data class LedgerPermissionResource(val permissionId: UUID)

@Location("/accounts")
class AccountResources()

@Location("/accounts/{accountId}")
data class AccountResource(val accountId: UUID)

@Location("/envelopes")
class EnvelopeResources()

@Location("/envelopes/{envelopeId}")
data class EnvelopeResource(val envelopeId: UUID)
