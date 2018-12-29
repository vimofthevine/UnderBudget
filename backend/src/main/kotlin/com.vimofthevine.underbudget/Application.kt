package com.vimofthevine.underbudget

import com.vimofthevine.underbudget.auth.*
import com.vimofthevine.underbudget.ledger.*

import java.util.UUID

import io.ktor.application.*
import io.ktor.features.*
import io.ktor.gson.*
import io.ktor.http.*
import io.ktor.locations.*
import io.ktor.response.*
import io.ktor.routing.*
import io.ktor.util.*

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
    
    val dbService = DbService(db)
    
	if (isDemo) {
    	transaction(db) {
            setupDemo(dbService)
    	}
    }
    
    install(DefaultHeaders)
    install(CallLogging)
    install(Locations)
    install(DataConversion) {
        convert<UUID> {
            decode { values, _ ->
            	values.singleOrNull()?.let { UUID.fromString(it) }
			}
            encode { value ->
            	when (value) {
                    null -> listOf()
                    is UUID -> listOf(value.toString())
                    else -> throw DataConversionException("Cannot convert $value as UUID")
                }
            }
        }
    }
    install(ContentNegotiation) {
        gson {
            
        }
    }
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
        ledger(dbService, auth = !isDemo)
    }
}