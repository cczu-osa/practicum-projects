<template>
  <div id="status">
    <h2>总览</h2>

    <div class="row">
      <table>
        <tr v-for="info in userInfo">
          <td> {{ info.name }} </td><td> {{ info.value }} </td>
        </tr>
        <tr v-for="info in sysInfo">
          <td> {{ info.name }} </td><td> {{ info.value }} </td>
        </tr>
      </table>
    </div>

  </div>
</template>

<script>
export default {
  name: 'status',
  data () {
    return {
      userInfo: [
        { name: '用户名', value: this.$parent.userName },
        { name: '访问秘钥', value: this.$parent.accessToken.substr(0, 15) + new Array(this.$parent.accessToken.length - 15).fill('*').join('')}
      ],
      sysInfo: [],
    }
  },
  methods: {
  },
  created: function() {
    let accessToken = this.$parent.accessToken;
    if (!accessToken || accessToken === '')
      this.$parent.tabNavigate(-1);

    this.$http.get(`/api/v1/status?token=${this.$parent.accessToken}`).then((res) => {
      this.sysInfo = res.body.server;
    });
  }
}
</script>

<style scoped>
  #status {
    max-width: 800px;
    margin: 0 auto;
  }
  h2 {
    font-size: 22px;
  }
  h3 {
    font-size: 16px;
    font-weight: bold;
  }
  h5 {
    font-size: 16px;
  }
  .row {
    margin-bottom: 0;
  }
</style>
