package com.vimofthevine.underbudget.ledger

import com.vimofthevine.underbudget.auth.*
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

fun Route.ledgerRoutes(db: DbService) {
    get<LedgerResources> {
        call.userId?.let {
			call.respond(Ledgers(db.transaction { findLedgersByUser(it) }))
        }
	}
    
    post<LedgerResources> {
        var ledger = call.receive<Ledger>()
        var id = db.transaction { createLedger(ledger) }
        call.respond(id)
    }
    
    get<LedgerResource> {
        /*
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
        */
	}
    
}