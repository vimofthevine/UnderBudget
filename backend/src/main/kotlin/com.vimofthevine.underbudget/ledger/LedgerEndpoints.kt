package com.vimofthevine.underbudget.ledger

import java.util.UUID

import io.ktor.application.*
import io.ktor.http.*
import io.ktor.locations.*
import io.ktor.response.*
import io.ktor.routing.*

import org.jetbrains.exposed.sql.*
import org.jetbrains.exposed.sql.transactions.*

data class LedgerResponse(val id: UUID, val name: String)

fun toLedger(dao: Ledger) = LedgerResponse(
    id = dao.id.value,
    name = dao.name
)

fun Routing.ledgerEndpoints(db: Database, auth: Boolean) {
    get<LedgerResources> {
        var ledgers: List<Ledger> = transaction(db) {
            if (auth) {
                listOf()
            } else {
				Ledger.all().toList()
            }
        }
        call.respond(ledgers.mapNotNull { toLedger(it) })
	}
    get<LedgerResource> {
        call.respondText("accounts for ledger ${it.ledger}", ContentType.Text.Html)
	}
}