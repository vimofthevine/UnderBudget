package com.vimofthevine.underbudget

import io.ktor.application.*
import io.ktor.http.*
import io.ktor.response.*
import io.ktor.routing.*

import org.jetbrains.exposed.sql.Database

val Application.dbUrl get() = environment.config.property("database.url").getString()
val Application.dbDriver get() = environment.config.property("database.driver").getString()
val Application.dbUser get() = environment.config.property("database.user").getString()
val Application.dbPassword get() = environment.config.property("database.password").getString()

fun Application.main() {
    val db = Database.connect(url = dbUrl, driver = dbDriver, user = dbUser, password = dbPassword)
    
    routing {
        get("/") {
            call.respondText("Hello, world!", ContentType.Text.Html)
        }
    }
}