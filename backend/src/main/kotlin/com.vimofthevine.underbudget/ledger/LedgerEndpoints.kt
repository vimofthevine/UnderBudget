package com.vimofthevine.underbudget.ledger

import io.ktor.application.*
import io.ktor.http.*
import io.ktor.locations.*
import io.ktor.response.*
import io.ktor.routing.*

import org.jetbrains.exposed.sql.Database

fun Routing.ledgerEndpoints(db: Database) {
    get<LedgerResources> {
		call.respondText("list of available ledgers", ContentType.Text.Html)
	}
    get<LedgerResource> {
        call.respondText("accounts for ledger ${it.ledger}", ContentType.Text.Html)
	}
}