package com.vimofthevine.underbudget.ledger

import com.vimofthevine.underbudget.DbService

import java.util.UUID

import io.ktor.application.*
import io.ktor.http.*
import io.ktor.locations.*
import io.ktor.request.*
import io.ktor.response.*
import io.ktor.routing.*

import org.jetbrains.exposed.sql.*
import org.jetbrains.exposed.sql.transactions.*

fun Route.ledgerEndpoints(db: DbService, auth: Boolean) {
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
        var ledger: Ledger? = db.transaction {
            if (auth) {
                null
            } else {
                getLedger(it.ledger)
            }
        }
        if (ledger != null) {
        	call.respond(ledger)
    	}
	}
    post<LedgerResources> {
        var ledger = call.receive<Ledger>()
        var id = db.transaction {
            create(ledger)
        }
        call.respond(id)
    }
}