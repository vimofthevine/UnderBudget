package com.vimofthevine.underbudget

import com.vimofthevine.underbudget.auth.*
import com.vimofthevine.underbudget.ledger.*

import io.ktor.application.*
import io.ktor.features.*
import io.ktor.http.*
import io.ktor.locations.*
import io.ktor.response.*
import io.ktor.routing.*

import org.jetbrains.exposed.sql.Database
import org.jetbrains.exposed.sql.transactions.transaction

val Application.isDemo get() = environment.config.property("ktor.deployment.demo").getString() == "true"
val Application.dbUrl get() = environment.config.property("database.url").getString()
val Application.dbDriver get() = environment.config.property("database.driver").getString()
val Application.dbUser get() = environment.config.property("database.user").getString()
val Application.dbPassword get() = environment.config.property("database.password").getString()

fun Application.main() {
    val db = Database.connect(url = dbUrl, driver = dbDriver, user = dbUser, password = dbPassword)
    transaction(db) {
        setupAuthTables()
        setupLedgerTables()
    }
    
	if (isDemo) {
    	transaction(db) {
            setupDemo()
    	}
    }
    
    install(DefaultHeaders)
    install(CallLogging)
    install(Locations)
    install(StatusPages) {
        exception<NotImplementedError> {
            call.respond(HttpStatusCode.NotImplemented)
        }
        exception<Throwable> { cause ->
            call.respond(HttpStatusCode.InternalServerError)
            throw cause
        }
    }
    
    routing {
        get("/") {
            call.respondText("Hello, world!", ContentType.Text.Html)
        }
        auth(db)
        ledger(db)
    }
}