package com.vimofthevine.underbudget.ledger

import io.ktor.locations.*

@Location("/ledgers")
class LedgerResources()

@Location("/ledgers/{ledger}")
data class LedgerResource(val ledger: String)

@Location("/accounts/{account}")
data class AccountResource(val account: String)

@Location("/envelopes/{envelope}")
data class EnvelopeResource(val envelope: String)
