import React from 'react'
import { NavLink } from 'react-router-dom';

export default function LinkApp() {

    return (
        <div>
            <div className="d-flex flex-column flex-shrink-0 p-3 text-white bg-dark" style={{ "width": "280px", "height": "100vh" }}>
                <hr />
                <ul className="nav nav-pills flex-column mb-auto">
                    <li className="nav-item ">
                        <NavLink to="/anime"
                            className="nav-link text-white"
                            activeClassName="active">
                            Animes </NavLink>
                    </li>
                    <li>
                        <NavLink to="/log"
                            className="nav-link text-white"
                            activeClassName="active"> Log </NavLink>
                    </li>
                </ul>
                <hr />
            </div>

        </div>
    )
}
