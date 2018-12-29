package com.vimofthevine.underbudget.ledger

import io.ktor.application.*
import io.ktor.http.*
import io.ktor.locations.*
import io.ktor.response.*
import io.ktor.routing.*

import org.jetbrains.exposed.sql.Database

fun Routing.ledger(db: Database) {
    get<LedgerResources> {
		call.respondText("list of available ledgers", ContentType.Text.Html)
	}
    get<LedgerResource> {
        call.respondText("accounts for ledger ${it.ledger}", ContentType.Text.Html)
	}
    get<AccountResource> {
		call.respondText("account details for ${it.account}", ContentType.Text.Html)
	}
    get<EnvelopeResource> {
        call.respondText("envelope details for ${it.envelope}", ContentType.Text.Html)
    }
}