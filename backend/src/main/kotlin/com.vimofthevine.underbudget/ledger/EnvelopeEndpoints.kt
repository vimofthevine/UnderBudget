package com.vimofthevine.underbudget.ledger

import io.ktor.application.*
import io.ktor.http.*
import io.ktor.locations.*
import io.ktor.response.*
import io.ktor.routing.*

import org.jetbrains.exposed.sql.Database

/*
fun Routing.envelopeEndpoints(db: Database) {
    get<EnvelopeResource> {
        call.respondText("envelope details for ${it.envelope}", ContentType.Text.Html)
    }
}
*/