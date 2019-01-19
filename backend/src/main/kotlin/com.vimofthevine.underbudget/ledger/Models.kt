package com.vimofthevine.underbudget.ledger

import java.util.UUID

data class CreateResponse(val id: UUID? = null, val error: String? = null)

data class Ledger(val name: String, val defaultCurrency: String, val id: UUID? = null)

data class Ledgers(val ledgers: List<Ledger>)