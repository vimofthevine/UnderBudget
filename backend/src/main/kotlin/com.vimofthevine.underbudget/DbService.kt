package com.vimofthevine.underbudget

import io.ktor.application.*

import kotlinx.coroutines.*

import org.jetbrains.exposed.sql.Database
import org.jetbrains.exposed.sql.transactions.transaction

class DbService(val db: Database) {
    
    suspend fun <T> transaction(body: DbService.() -> T): T =
    	withContext(Dispatchers.IO) {
            transaction(db) { body() }
        }
    
}

val Application.dbUrl get() = environment.config.property("database.url").getString()
val Application.dbDriver get() = environment.config.property("database.driver").getString()
val Application.dbUser get() = environment.config.property("database.user").getString()
val Application.dbPassword get() = environment.config.property("database.password").getString()

fun Application.createDbService() =
	DbService(Database.connect(url = dbUrl, driver = dbDriver, user = dbUser, password = dbPassword))
