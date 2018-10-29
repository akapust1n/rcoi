import React, { Component } from 'react'
import Article from '../Article'
import { _parseJSON, isEmpty } from "../../HttpAssist"
import { Pager } from "react-bootstrap"
import './style.css'

export default class Titles extends Component {
    constructor(props) {
        super(props);
        this.handleClick = props.handleClick;

        this.state = {
            page: 1
        };
        this.page = 1;
        this.url = "http://localhost:8080/titles?page=";
        this.nextPage = this.nextPage.bind(this);
        this.prevPage = this.prevPage.bind(this);
    }
    componentDidMount() {
        const finalUrl = this.url + this.page;
        console.log("will mount");

        const request = async () => {
            const res = await fetch(finalUrl);
            if (res.status === 200) {
                console.log("newsLoaded");
                let news = await _parseJSON(res);
                console.log("news_", news);
                this.setState({ news: news });
            } else {
                alert("cant load news!");
            }
        }
        request();
    }
    nextPage() {
        this.page += 1;
        this.setState({ isLoaded: false }, () => {
            this.componentDidMount();
        });
    }
    prevPage() {
        this.page = this.page > 1 ? this.page - 1 : this.page;
        this.setState({ isLoaded: false }, () => {
            this.componentDidMount();
        });
    }
    render() {
        console.log("render_", this.state.news);
        if (this.state.news === undefined || isEmpty(this.state.news)) {
            return (
                'loading'
            )
        }
        else {
            const news = this.state.news;
            const articleElements = news.map((article, index) =>
                <li key={article.ID} className="article-list__li">
                    <Article article={article} id={article.ID} handleClick={this.handleClick} />
                </li>
            )
            let disabledNext = articleElements.length !== 10
            let disabledPrev = this.page === 1;

            let pager = (<Pager>
                <Pager.Item disabled={disabledPrev} previous onClick={this.prevPage}>
                    &larr; Previous
            </Pager.Item>
                <Pager.Item disabled={disabledNext} next onClick={this.nextPage}>
                    Next &rarr;
            </Pager.Item>
            </Pager>)
            return (
                <div>
                    {pager}
                    < ul >
                        {articleElements}
                    </ul >
                </div>
            )
        }

    }
}

