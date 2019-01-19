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
        call.userId?.let {
        	val ledger = call.receive<Ledger>()
        	if ((ledger.name == null) or (ledger.defaultCurrency == null)) {
            	call.respond(HttpStatusCode.BadRequest,
                             CreateResponse(error = "Missing required field(s)"))
            } else if (ledger.name.length > 128) {
            	call.respond(HttpStatusCode.BadRequest,
                             CreateResponse(error = "Ledger name must be less than 128 characters in length"))
            } else if (!Currencies.isValid(ledger.defaultCurrency)) {
                call.respond(HttpStatusCode.BadRequest,
                             CreateResponse(error = "Invalid currency specified"))
        	} else {
        		val id = db.transaction { createLedger(ledger) }
        		call.respond(HttpStatusCode.Created, CreateResponse(id = id))
        	}
        }
    }
    
    get<LedgerResource> { req ->
        call.userId?.let { userId ->
			val ledger = db.transaction {
                if (hasLedgerPermission(req.ledgerId, userId)) {
                    findLedgerById(req.ledgerId)
                } else {
                    null
                }
            } 
            if (ledger == null) {
                call.respond(HttpStatusCode.Forbidden)
            } else {
                call.respond(ledger)
            }
        }
	}
    
    get<LedgerPermissionResources>() {
        // use ledgerId from query params
    }
    
    post<LedgerPermissionResources>() {
        
    }
    
    delete<LedgerPermissionResource>() {
        
    }
    
}