package com.vimofthevine.underbudget.ledger

import io.ktor.locations.*

import java.util.UUID

@Location("/ledgers")
class LedgerResources()

@Location("/ledgers/{ledger}")
data class LedgerResource(val ledger: UUID)

@Location("/accounts/{account}")
data class AccountResource(val account: UUID)

@Location("/envelopes/{envelope}")
data class EnvelopeResource(val envelope: UUID)
