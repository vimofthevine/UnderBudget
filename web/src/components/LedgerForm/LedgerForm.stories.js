import React from 'react';
import { storiesOf } from '@storybook/react';
import { Formik } from 'formik';
import LedgerForm from './LedgerForm';

storiesOf('Ledger|LedgerForm', module)
  .add('default', () => (
    <Formik initialValues={{ currency: 'USD' }}>
      <LedgerForm />
    </Formik>
  ))
  .add('with ledger', () => (
    <Formik initialValues={{ name: 'Foreign', currency: 'UAH' }}>
      <LedgerForm />
    </Formik>
  ));
