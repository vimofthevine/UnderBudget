package com.vimofthevine.underbudget.auth

import com.google.gson.Gson
import com.vimofthevine.underbudget.TestFixture

import io.ktor.http.*
import io.ktor.server.testing.*

import org.amshove.kluent.*
import org.junit.jupiter.api.Test

class CreateUserTest : TestFixture() {
    @Test fun `should reject short username`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Post
            uri = "/users"
            setBody(
                Gson().toJson(mapOf(
                    "name" to "bob",
                    "email" to "bob@test.com",
                    "password" to "testPassword"
                ))
            )
        }
        
        req.requestHandled shouldBe true
        req.response.content.shouldNotBeNullOrBlank() shouldContain "Username must be at least 6 characters in length"
    }
    
    @Test fun `should reject long username`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Post
            uri = "/users"
            setBody(
                Gson().toJson(mapOf(
                    "name" to "bobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobbybobby",
                    "email" to "bob@test.com",
                    "password" to "testPassword"
                ))
            )
        }
        
        req.requestHandled shouldBe true
        req.response.content.shouldNotBeNullOrBlank() shouldContain "Username must be less than 128 characters in length"
    }
    
    @Test fun `should reject invalid username`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Post
            uri = "/users"
            setBody(
                Gson().toJson(mapOf(
                    "name" to "bobby!",
                    "email" to "bob@test.com",
                    "password" to "testPassword"
                ))
            )
        }
        
        req.requestHandled shouldBe true
        req.response.content.shouldNotBeNullOrBlank() shouldContain "Username must contain only letters, numbers, dots, or underscores"
    }
    
    @Test fun `should reject short password`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Post
            uri = "/users"
            setBody(
                Gson().toJson(mapOf(
                    "name" to "robert",
                    "email" to "bob@test.com",
                    "password" to "test"
                ))
            )
        }
        
        req.requestHandled shouldBe true
        req.response.content.shouldNotBeNullOrBlank() shouldContain "Password must be at least 12 characters in length"
    }
    
    @Test fun `should reject duplicate username`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Post
            uri = "/users"
            setBody(
                Gson().toJson(mapOf(
                    "name" to "testUser",
                    "email" to "bob@test.com",
                    "password" to "password123456"
                ))
            )
        }
        
        req.requestHandled shouldBe true
        req.response.content.shouldNotBeNullOrBlank() shouldContain "Username is already in use"
    }
    
    @Test fun `should reject duplicate email`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Post
            uri = "/users"
            setBody(
                Gson().toJson(mapOf(
                    "name" to "robert",
                    "email" to "user@test.com",
                    "password" to "password123456"
                ))
            )
        }
        
        req.requestHandled shouldBe true
        req.response.content.shouldNotBeNullOrBlank() shouldContain "User with given email address already exists"
    }
    
    @Test fun `should create user`() = withServer {
        val req = handleRequest {
            method = HttpMethod.Post
            uri = "/users"
            setBody(
                Gson().toJson(mapOf(
                    "name" to "robert",
                    "email" to "bob@test.com",
                    "password" to "password123456"
                ))
            )
        }
        
        req.requestHandled shouldBe true
        req.response.content.shouldNotBeNullOrBlank() shouldNotContain "error"
        req.response.content.shouldNotBeNullOrBlank() shouldContain "userId"
    }
}