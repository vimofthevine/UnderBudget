import React from 'react';
import { Form, Field } from 'formik';
import CurrencyInputField from '../CurrencyInputField/CurrencyInputField';
import TextInputField from '../TextInputField/TextInputField';

const LedgerForm = () => (
  <Form>
    <Field
      name='name'
      label='Name'
      autoFocus
      component={TextInputField}
    />
    <Field
      name='defaultCurrency'
      label='Default currency'
      helperText='Default currency to use for accounts and envelopes in the ledger'
      component={CurrencyInputField}
    />
  </Form>
);

export default LedgerForm;
