import React from 'react';
import { storiesOf } from '@storybook/react';
import { Formik } from 'formik';
import LedgerForm from './LedgerForm';

storiesOf('Ledger|Ledgers/LedgerForm', module)
  .add('new ledger', () => (
    <Formik initialValues={{ currency: 'USD' }}>
      <LedgerForm />
    </Formik>
  ))
  .add('existing ledger', () => (
    <Formik initialValues={{ name: 'Foreign', currency: 'UAH' }}>
      <LedgerForm />
    </Formik>
  ));
