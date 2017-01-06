<template>
  <div id="news">
    <h2>新闻政策管理</h2>
    <div id="newsMain">
      <!-- 新闻列表 -->
      <div v-if="showing === 'list'">
        <h5>对新闻政策进行管理操作。</h5>
        <div class="row">
          <table class="highlight">
            <thead>
              <tr>
                <th>新闻政策编号</th>
                <th>新闻名称</th>
                <th>新闻内容</th>
                <th>新闻日期</th>
                <th>新闻类型 ID</th>
                <th>新闻状态 ID</th>
                <th>文件类型 ID</th>
                <th>发布的执法人员 ID</th>
              </tr>
            </thead>
            <tbody>
              <tr v-for="(news, index) in newsArray" v-on:click="newsEdit(index)">
                <td> {{ news.id }} </td>
                <td> {{ news.title }} </td>
                <td> {{ news.content}} </td>
                <td> {{ news.date }} </td>
                <td> {{ newsTypeArray.filter(item => item.id === news.type_id)[0].value }} </td>
                <td> {{ newsStatusArray.filter(item => item.id === news.status_id)[0].value }}</td>
                <td> {{ fileTypeArray.filter(item => item.id === news.file_type_id)[0].value }} </td>
                <td> {{ officerArray.filter(item => item.id === news.publish_officer_id)[0].name }} </td>
              </tr>
            </tbody>
          </table>
        </div>
        <div class="row frm">
          <a class="waves-effect waves-light btn" v-on:click="newsAdd">新增</a>
        </div>
      </div>
      <!-- 新闻编辑 -->
      <div v-if="showing === 'edit'">
        <h5>创建/编辑新闻政策。</h5>
        <div class="row frm">
          <div class="input-field col s4">
            <input id="id" type="text" class="validate" v-model="editing.id" disabled />
            <label for="id">新闻政策编号</label>
          </div>
        </div>
        <div class="row frm">
          <div class="input-field col s4">
            <input id="title" type="text" class="validate" v-model="editing.title" />
            <label for="title">新闻名称</label>
          </div>
          <div class="input-field col s4">
            <input id="date" type="text" class="validate" v-model="editing.date" />
            <label for="date">新闻日期</label>
          </div>
          <div class="input-field col s4">
            <select id="typeSelector">
              <option value="" disabled selected>选择类型</option>
              <option v-for="(type, index) in newsTypeArray" :value="type.id"> {{ type.value }} </option>
            </select>
            <label>新闻类型</label>
          </div>
        </div>
        <div class="row frm">
          <div class="input-field col s4">
            <select id="statusSelector">
              <option value="" disabled selected>选择状态</option>
              <option v-for="(status, index) in newsStatusArray" :value="status.id"> {{ status.value }} </option>
            </select>
            <label>新闻状态</label>
          </div>
          <div class="input-field col s4">
            <select id="fileTypeSelector">
              <option value="" disabled selected>选择文件类型</option>
              <option v-for="(fileType, index) in fileTypeArray" :value="fileType.id"> {{ fileType.value }} </option>
            </select>
            <label>新闻文件类型</label>
          </div>
          <div class="input-field col s4">
            <select id="officerSelector">
              <option value="" disabled selected>选择执法人员</option>
              <option v-for="(officer, index) in officerArray" :value="officer.id"> {{ officer.name }} </option>
            </select>
            <label>发布的执法人员</label>
          </div>
        </div>
        <div class="row frm">
          <div class="input-field col s12">
            <textarea id="content" type="text" class="materialize-textarea" v-model="editing.content"></textarea>
            <label for="content">新闻内容</label>
          </div>
        </div>
        <div class="row frm">
          <!-- button -->
          <div class="input-field inline col s12">
            <a class="waves-effect waves-light btn" v-on:click="back">返回</a>
            <a class="waves-effect waves-light btn" v-on:click="newsSave">保存</a>
            <a class="waves-effect waves-light btn red right" v-on:click="newsDelete" v-if="this.editing.id !== ''">删除</a>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'news',
  data () {
    return {
      showing: 'list',
      newsArray: [],
      newsTypeArray: [],
      newsStatusArray: [],
      fileTypeArray: [],
      officerArray: [],
      editing: {},
    }
  },
  methods: {
    refresh: function() {
      this.$http.get(`/api/v1/news?token=${this.$parent.accessToken}`).then((response) => {
        this.newsArray = response.body.dataset;
      })
    },
    switchto: function (dest, callback) {
      let content = document.querySelector('#newsMain');
      content.style.opacity = 0;
      setTimeout(() => {
        this.showing = dest;
        if (callback) setTimeout(() => {callback();}, 0);
        content.style.opacity = 1;
      }, 100);
    },
    newsAdd: function() {
      this.editing = {
        id: '',
        date: '',
        title: '',
        content: '',
        type_id: '',
        status_id: '',
        file_type_id: '',
        publish_officer_id: '',
      }
      this.switchto('edit');
    },
    newsEdit: function(index) {
      this.editing = JSON.parse(JSON.stringify(this.newsArray[index]));
      this.switchto('edit', () => {
        document.querySelector('#typeSelector').value = this.editing.type_id;
        document.querySelector('#statusSelector').value = this.editing.status_id;
        document.querySelector('#fileTypeSelector').value = this.editing.file_type_id;
        document.querySelector('#officerSelector').value = this.editing.publish_officer_id;
      });
    },
    newsDelete: function() {
      if (this.editing.id !== '') {
        this.$http.delete(`/api/v1/news?token=${this.$parent.accessToken}&id=${this.editing.id}`).then((data) => {
          if (data.body.status === 'ok') {
            this.refresh();
            this.back();
          }
          else {
            Materialize.toast('权限不足，只有管理员可以删除执法人员信息', 4000)
          }
        })
      }
    },
    newsSave: function() {
      this.editing.type_id = document.querySelector('#typeSelector').value;
      this.editing.status_id = document.querySelector('#statusSelector').value;
      this.editing.file_type_id = document.querySelector('#fileTypeSelector').value;
      this.editing.publish_officer_id = document.querySelector('#officerSelector').value;
      let query = [];
      for (let _ in this.editing) {
        if (_ === 'id' && this.editing.id === '')
          continue;
        query.push(`${_}=${encodeURIComponent(this.editing[_])}`);
      }

      query = query.join('&');
      console.log(query);
      let finishHandler = (data) => {
        if (data.body.status === 'ok') {
          this.refresh();
          this.back();
        }
        else {
          Materialize.toast('权限不足，只有管理员和执法人员可以修改和创建新闻政策信息', 4000)
        }
      }
      if (this.editing.id === '') {
        this.$http.put(`/api/v1/news?token=${this.$parent.accessToken}&${query}`).then(finishHandler)
      }
      else {
        this.$http.post(`/api/v1/news?token=${this.$parent.accessToken}&${query}`).then(finishHandler)
      }
    },
    back: function() {
      this.switchto('list', () => { this.refresh(); });
    },
  },
  created: function() {
    let accessToken = this.$parent.accessToken;
    if (!accessToken || accessToken === '')
      this.$parent.tabNavigate(-1);

    this.$http.get(`/api/v1/dict?token=${this.$parent.accessToken}&category=NEWS_TYPE`).then((response) => {
      this.newsTypeArray = response.body.dataset;
    })
    this.$http.get(`/api/v1/dict?token=${this.$parent.accessToken}&category=NEWS_STATUS`).then((response) => {
      this.newsStatusArray = response.body.dataset;
    })
    this.$http.get(`/api/v1/dict?token=${this.$parent.accessToken}&category=FILE_TYPE`).then((response) => {
      this.fileTypeArray = response.body.dataset;
    })
    this.$http.get(`/api/v1/officer?token=${this.$parent.accessToken}`).then((response) => {
      this.officerArray = response.body.dataset;
    })


    this.refresh();
  },
  watch: {
    showing: function(val) {
      setTimeout(() => {
        $('select').material_select();
        Materialize.updateTextFields(); 
      }, 0);
    }
  }
}
</script>

<style scoped>
  #news {
    max-width: 1000px;
    margin: 0 auto;
  }
  #newsMain {
    transition: all ease 0.1s;
  }
  h2 {
    font-size: 22px;
  }
  h5 {
    font-size: 16px;
  }
  div.frm {
    margin-bottom: 0;
  }
</style>