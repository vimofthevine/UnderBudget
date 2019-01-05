package com.vimofthevine.underbudget.auth

import com.vimofthevine.underbudget.TestFixture

import org.amshove.kluent.*
import org.junit.jupiter.api.Test

class CreateUserTest : TestFixture() {
    @Test fun `should pass`() {
        "hello" shouldEqual "hello"
    }
    
    @Test fun `should fail`() {
        "hello" shouldEqual "Hello"
    }
}