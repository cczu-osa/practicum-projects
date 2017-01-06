import Vue from 'vue';
import vueResource from 'vue-resource';
import App from './App';

import 'materialize-css/js/velocity.min.js';
import 'materialize-css/bin/materialize.css';
import 'materialize-css/bin/materialize.js';

Vue.use(vueResource);

new Vue({
    el: '#app',
    template: '<App />',
    components: { App }
})