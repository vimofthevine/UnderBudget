import React, { Component } from 'react'
import AuthService from '../auth/AuthService'

export default function withAuth(AuthComponent) {
    const auth = new AuthService()
    
    return class AuthWrapped extends Component {
        constructor() {
            super()
            this.state = {
                user: null
            }
        }
        
        componentWillMount() {
            if (!auth.loggedIn()) {
                this.props.history.replace('/login')
            } else {
                try {
                    const profile = auth.getProfile()
                    this.setState({
                        user: profile
                    })
                } catch(err) {
                    auth.logout()
                	this.props.history.replace('/login')
                }
            }
        }
        
        render() {
            if (this.state.user) {
                return (
                    <AuthComponent history={this.props.history} user={this.state.user} />
                )
            } else {
                return null
            }
        }
    }
}