package com.vimofthevine.underbudget.ledger

import com.vimofthevine.underbudget.auth.User

import java.util.UUID

data class CreateResponse(val id: UUID? = null, val error: String? = null)

data class Ledger(val name: String, val defaultCurrency: String, val id: UUID? = null)

data class Ledgers(val ledgers: List<Ledger>)

data class LedgerPermissionInput(val ledgerId: UUID, val userId: UUID)

data class LedgerPermission(val id: UUID, val ledger: Ledger, val user: User)

data class LedgerPermissions(val permissions: List<LedgerPermission>)