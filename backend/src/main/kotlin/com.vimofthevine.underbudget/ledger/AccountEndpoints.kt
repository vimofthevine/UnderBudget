package com.vimofthevine.underbudget.ledger

import io.ktor.application.*
import io.ktor.http.*
import io.ktor.locations.*
import io.ktor.response.*
import io.ktor.routing.*

import org.jetbrains.exposed.sql.Database

/*
fun Routing.accountEndpoints(db: Database) {
    get<AccountResource> {
		call.respondText("account details for ${it.account}", ContentType.Text.Html)
	}
}
*/