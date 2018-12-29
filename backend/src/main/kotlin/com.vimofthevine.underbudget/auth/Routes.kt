package com.vimofthevine.underbudget.auth

import io.ktor.application.*
import io.ktor.http.*
import io.ktor.response.*
import io.ktor.routing.*

import org.jetbrains.exposed.sql.Database

fun Routing.auth(db: Database) {
    get("/register") {
        call.respondText("Register page", ContentType.Text.Html)
    }
}