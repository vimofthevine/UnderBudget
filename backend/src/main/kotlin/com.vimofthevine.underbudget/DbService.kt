package com.vimofthevine.underbudget

import kotlinx.coroutines.*

import org.jetbrains.exposed.sql.Database
import org.jetbrains.exposed.sql.transactions.transaction

class DbService(val db: Database) {
    
    suspend fun <T> transaction(body: DbService.() -> T): T =
    	withContext(Dispatchers.IO) {
            transaction(db) { body() }
        }
    
}