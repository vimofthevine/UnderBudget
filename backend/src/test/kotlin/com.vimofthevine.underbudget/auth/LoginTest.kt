package com.vimofthevine.underbudget.auth

import com.google.gson.Gson
import com.vimofthevine.underbudget.TestFixture

import io.ktor.http.*
import io.ktor.server.testing.*

import org.amshove.kluent.*
import org.junit.jupiter.api.Test

data class TokenResponse(val token: String)

class LoginTest : TestFixture() {
    @Test fun `should reject invalid username`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Post
            uri = "/tokens"
            setBody(
                Gson().toJson(mapOf(
                    "name" to "incrhulk",
                    "password" to "strongestAvenger"
                ))
            )
        }
        
        req.requestHandled shouldBe true
        req.response.status() shouldBe HttpStatusCode.BadRequest
        req.response.content.shouldNotBeNullOrBlank() shouldContain "Invalid login credentials"
    }
    
    @Test fun `should reject invalid password`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Post
            uri = "/tokens"
            setBody(
                Gson().toJson(mapOf(
                    "name" to "testUser",
                    "password" to "notMyPassword"
                ))
            )
        }
        
        req.requestHandled shouldBe true
        req.response.status() shouldBe HttpStatusCode.BadRequest
        req.response.content.shouldNotBeNullOrBlank() shouldContain "Invalid login credentials"
    }
    
    @Test fun `should create token for correct credentials`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Post
            uri = "/tokens"
            setBody(
                Gson().toJson(mapOf(
                    "name" to "testUser",
                    "password" to "testPassword"
                ))
            )
        }
        
        req.requestHandled shouldBe true
        req.response.status() shouldBe HttpStatusCode.Created
        req.response.content.shouldNotBeNullOrBlank() shouldContain "token"
    }
    
    @Test fun `should create unique token for same user`() = withServer {
        val req1 = handleRequest {
            method = HttpMethod.Post
            uri = "/tokens"
            setBody(
                Gson().toJson(mapOf(
                    "name" to "testUser",
                    "password" to "testPassword"
                ))
            )
        }
        
        req1.requestHandled shouldBe true
        val token1 = Gson().fromJson(req1.response.content.shouldNotBeNullOrBlank(),
                                     TokenResponse::class.java)
        
        val req2 = handleRequest {
            method = HttpMethod.Post
            uri = "/tokens"
            setBody(
                Gson().toJson(mapOf(
                    "name" to "testUser",
                    "password" to "testPassword"
                ))
            )
        }
        
        req2.requestHandled shouldBe true
        val token2 = Gson().fromJson(req2.response.content.shouldNotBeNullOrBlank(),
                                     TokenResponse::class.java)
        
        token1 shouldNotEqual token2
    }
}