import React from 'react'
import { Route, Switch } from 'react-router-dom';
import CreateLedger from './CreateLedger'

export default function Ledgers() {
  return (
    <Switch>
      <Route path="/ledgers/create" component={CreateLedger} />
    </Switch>
  )
}