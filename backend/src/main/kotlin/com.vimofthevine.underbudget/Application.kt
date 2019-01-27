package com.vimofthevine.underbudget

import com.vimofthevine.underbudget.auth.*
import com.vimofthevine.underbudget.ledger.*

import java.lang.reflect.Modifier
import java.util.UUID

import io.ktor.application.*
import io.ktor.auth.*
import io.ktor.auth.jwt.*
import io.ktor.features.*
import io.ktor.gson.*
import io.ktor.http.*
import io.ktor.locations.*
import io.ktor.response.*
import io.ktor.routing.*
import io.ktor.util.*

import org.jetbrains.exposed.sql.transactions.transaction
import org.joda.time.DateTime

val Application.isDemo get() = environment.config.property("ktor.deployment.demo").getString() == "true"

fun Application.main(dbService: DbService = createDbService(),
                     passwdService: Passwords = createPasswords(),
                     jwtService: JwtService = createJwtService()) {
    transaction(dbService.db) {
        setupAuthTables()
        setupLedgerTables()
    }
    
	if (isDemo) {
    	transaction(dbService.db) {
            setupDemo(dbService)
    	}
    }
    
    install(DefaultHeaders)
    install(CallLogging)
    install(CORS) {
		header(HttpHeaders.Authorization)
		method(HttpMethod.Delete)
		method(HttpMethod.Options)
		anyHost()
		allowCredentials = true
	}
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
            excludeFieldsWithModifiers(Modifier.TRANSIENT)
            setDateFormat("yyyy-MM-dd'T'HH:mm:ssXXX")
            setPrettyPrinting()
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
    install(Authentication) {
        jwt("jwt") {
            realm = jwtRealm
            verifier(jwtService.verifier)
            validate {
                val user = it.payload.getClaim("userId")?.asString()?.let {
                    dbService.transaction { findUserById(UUID.fromString(it)) }
                }
                val token = dbService.transaction { findTokenByJwtId(it.payload.getId()) }
                if ((user != null) and (token != null)) JWTPrincipal(it.payload) else null
            }
        }
    }
    
    routing {
        auth(dbService, passwdService, jwtService)
        authenticate("jwt") {
            ledger(dbService)
        }
    }
}