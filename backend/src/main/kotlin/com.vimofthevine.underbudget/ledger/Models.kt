package com.vimofthevine.underbudget.ledger

import java.util.UUID

data class Ledger(val id: UUID?, val name: String, val defaultCurrency: String)

data class Ledgers(val ledgers: List<Ledger>)