import React from 'react';
import { Form, Field } from 'formik';
import CurrencyInputField from '../CurrencyInputField/CurrencyInputField';
import TextInputField from '../TextInputField/TextInputField';

const LedgerForm = () => (
  <Form>
    <Field
      id='name'
      name='name'
      label='Name'
      autoFocus
      component={TextInputField}
    />
    <Field
      id='currency'
      name='currency'
      label='Default currency'
      helperText='Default currency to use for accounts and envelopes in the ledger'
      component={CurrencyInputField}
    />
  </Form>
);

export default LedgerForm;
