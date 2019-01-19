package com.vimofthevine.underbudget.ledger

import com.google.gson.Gson
import com.vimofthevine.underbudget.TestFixture

import io.ktor.http.*
import io.ktor.server.testing.*

import org.amshove.kluent.*
import org.junit.jupiter.api.Test

class CreateLedgerTest : TestFixture() {
    @Test fun `should reject unauthenticated`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Post
            uri = "/ledgers"
            setBody(
                Gson().toJson(mapOf(
                    "name" to "Ledger",
                    "defaultCurrency" to "USD"
                ))
            )
        }
        
        req.requestHandled shouldBe true
        req.response.status() shouldEqual HttpStatusCode.Unauthorized
    }
    
    @Test fun `should reject missing name`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Post
            uri = "/ledgers"
            addHeader("Authorization", "Bearer ${createToken()}")
            setBody(
                Gson().toJson(mapOf(
                    "callMe" to "Ledger",
                    "defaultCurrency" to "USD"
                ))
            )
        }
        
        req.requestHandled shouldBe true
        req.response.let {
            it.status() shouldBe HttpStatusCode.BadRequest
            it.content.shouldNotBeNullOrBlank() shouldContain "Missing required field(s)"
        }
    }
    
    @Test fun `should reject missing default currency`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Post
            uri = "/ledgers"
            addHeader("Authorization", "Bearer ${createToken()}")
            setBody(
                Gson().toJson(mapOf(
                    "name" to "Ledger",
                    "currency" to "USD"
                ))
            )
        }
        
        req.requestHandled shouldBe true
        req.response.let {
            it.status() shouldBe HttpStatusCode.BadRequest
            it.content.shouldNotBeNullOrBlank() shouldContain "Missing required field(s)"
        }
    }
    
    @Test fun `should reject long ledger name`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Post
            uri = "/ledgers"
            addHeader("Authorization", "Bearer ${createToken()}")
            setBody(
                Gson().toJson(mapOf(
                    "name" to "Ledgerwithareallyreallyreallyreallyreallyreallyreallyreallyreallyreallyreallyreallyreallyreallyreallyreallyreallyreallyreallyreallylongname",
                    "defaultCurrency" to "USD"
                ))
            )
        }
        
        req.requestHandled shouldBe true
        req.response.let {
            it.status() shouldBe HttpStatusCode.BadRequest
            it.content.shouldNotBeNullOrBlank() shouldContain "Ledger name must be less than 128 characters"
        }
    }
    
    @Test fun `should reject invalid currency`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Post
            uri = "/ledgers"
            addHeader("Authorization", "Bearer ${createToken()}")
            setBody(
                Gson().toJson(mapOf(
                    "name" to "Ledger",
                    "defaultCurrency" to "Dollars"
                ))
            )
        }
        
        req.requestHandled shouldBe true
        req.response.let {
            it.status() shouldBe HttpStatusCode.BadRequest
            it.content.shouldNotBeNullOrBlank() shouldContain "Invalid currency specified"
        }
    }
    
    @Test fun `should create ledger`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Post
            uri = "/ledgers"
            addHeader("Authorization", "Bearer ${createToken()}")
            setBody(
                Gson().toJson(mapOf(
                    "name" to "Ledger",
                    "defaultCurrency" to "USD"
                ))
            )
        }
        
        req.requestHandled shouldBe true
        req.response.let {
            it.status() shouldBe HttpStatusCode.Created
            it.content.shouldNotBeNullOrBlank() shouldContain "id"
        }
    }
}