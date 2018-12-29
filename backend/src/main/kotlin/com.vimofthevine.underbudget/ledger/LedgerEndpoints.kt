package com.vimofthevine.underbudget.ledger

import com.vimofthevine.underbudget.DbService

import java.util.UUID

import io.ktor.application.*
import io.ktor.http.*
import io.ktor.locations.*
import io.ktor.response.*
import io.ktor.routing.*

import org.jetbrains.exposed.sql.*
import org.jetbrains.exposed.sql.transactions.*

fun Routing.ledgerEndpoints(db: DbService, auth: Boolean) {
    get<LedgerResources> {
        var ledgers: List<Ledger> = db.transaction {
            if (auth) {
                listOf()
            } else {
                getLedgers()
            }
        }
        call.respond(ledgers)
	}
    get<LedgerResource> {
        call.respondText("accounts for ledger ${it.ledger}", ContentType.Text.Html)
	}
}