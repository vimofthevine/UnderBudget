package com.vimofthevine.underbudget.ledger

import java.util.UUID

import io.ktor.application.*
import io.ktor.http.*
import io.ktor.locations.*
import io.ktor.response.*
import io.ktor.routing.*

import org.jetbrains.exposed.sql.*
import org.jetbrains.exposed.sql.transactions.*

fun Routing.ledgerEndpoints(db: Database, service: LedgerService, auth: Boolean) {
    get<LedgerResources> {
        var ledgers: List<Ledger> = transaction(db) {
            if (auth) {
                listOf()
            } else {
                service.getLedgers()
            }
        }
        call.respond(ledgers)
	}
    get<LedgerResource> {
        call.respondText("accounts for ledger ${it.ledger}", ContentType.Text.Html)
	}
}