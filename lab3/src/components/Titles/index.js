import React, { Component } from 'react'
import Article from '../Article'
import _parseJSON from "../../HttpAssist"
import './style.css'

export default class Titles extends Component {
    constructor(props) {
        super(props);
        this.handleClick = props.handleClick;

        this.state = {
            page: 1
        };
        this.url = "http://localhost:8080/titles";
    }
    componentWillMount() {
        const finalUrl = this.url + "?page=1";
        const request = async () => {
            const res = await fetch(finalUrl);
            if (res.status === 200) {
                let news = await _parseJSON(res);
                this.setState({ news: news });
            } else {
                alert("cant load news!");
            }
        }
        request();
    }
    render() {
        if (this.state.news == undefined) {
            return (
                'loading'
            )
        }
        else {

        }
        const news = this.state.news;
        const articleElements = news.map((article, index) =>
            <li key={article.ID} className="article-list__li">
                <Article article={article} id={article.ID} handleClick={this.handleClick} />
            </li>
        )

        return (
            <ul>
                {articleElements}
            </ul>
        )

    }
}

